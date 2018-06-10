Ժ�Ҍ�ܟ�Փ�Л�第二次作业

2015011335 贾越凯

## 实现 keccak 算法、AES 算法（CBC模式）

本题的代码位于 `AES/src` 目录。各文件用途如下：

* `src/utils.h`：实现了一些实用函数；
* `src/bytearray.[cpp|h]`：实现了字节数组类；
* `src/aes.[cpp|h]`：实现了 AES 算法；
* `src/aesconst.h`：AES 算法中的一些常量，如 S-Box；
* `src/main.cpp`：主程序；
* `src/test.cpp`：测试程序；
* `src/CMakeLists.txt`：CMake 配置文件；
* `README.md`：构建、运行方法。

### keccak 算法

### AES 算法

AES 算法的实现在 `aes.cpp` 中。其具体原理可在课件中找到，主要由下面几个操作构成：

* 密钥扩展(`KeyExpansion`)；
* 轮密钥加(`AddRoundKey`)；
* 字节替换(`SubBytes`)；
* 行移位(`ShiftRows`)；
* 列混合(`MixColumns`)。

我实现版本的拥有如下特性：

* 支持所有长度(128、192、256)的密钥；
* 支持 ECB 模式和 CBC 模式；
* 当用户给定的密钥长度不足时，用 0 填充；
* 使用 PKCS#7 填充明文数据块；
* 加密/解密速度高达 210 MiB/s。

#### 算法加速

直接使用课件中的方法实现，速度约为 30 Mib/s。我参考了开源的 [crypto++](https://cryptopp.com/) 库中的实现，将其优化到了 200 MiB/s。

提升速度的思路是：将对 4 个单字节的操作合并为对一个字(32 位)的操作；将轮密钥加、字节替换、行移位、列混合这四个操作合并为一个；预先计算中间过程中的常量。

##### 合并操作

先考虑如何合并字节替换、行移位这两个操作。如图所示，左图中的状态经过行移位操作后变为右图中的状态，其中状态的每一列可以用一个 4 字节的字来表示，一个状态就可以用 4 个字表示(行编号小的为低位)。考虑旧状态的每一列对新状态的影响，在新状态中，来自旧状态同一列的部分用相同颜色表示。

![](img/1.png)

对旧状态的每一列，依次计算它对新状态每一列的贡献，伪代码如下：

```js
// 计算旧状态的 t 列对新状态每一列的贡献
function QUARTER_ROUND_E(t, a, b, c, d) {
    set_byte(d, 0, S_Box[get_byte(t, 0)]);
    set_byte(c, 1, S_Box[get_byte(t, 1)]);
    set_byte(b, 2, S_Box[get_byte(t, 2)]);
    set_byte(a, 3, S_Box[get_byte(t, 3)]);
}

// 旧状态为 (s0, s1, s2, s3)，新状态为 (t0, t1, t2, t3)
function SubBytes_ShiftRows(s0, s1, s2, s3, t0, t1, t2, t3) {
    QUARTER_ROUND_E(s3, t0, t1, t2, t3);
    QUARTER_ROUND_E(s2, t3, t0, t1, t2);
    QUARTER_ROUND_E(s1, t2, t3, t0, t1);
    QUARTER_ROUND_E(s0, t1, t2, t3, t0);
}
```

其中 `set_byte(word w, byte n, byte b)` 表示将 `w` 从低到高的第 `n` 字节设为 `b`，`get_byte(word w, byte n)` 表示获取 `w` 从低到高的第 `n` 字节。

##### 合并列混合

列混合操作涉及有限域上的乘法，需要用到大量异或运算，实验也发现发现这部分的运算速度最慢。

直接计算列混合的公式为

$$
\left[\begin{matrix}
S'_{0,c} \\
S'_{1,c} \\
S'_{2,c} \\
S'_{3,c} \\
\end{matrix}\right]
=
\left[\begin{matrix}
02 & 03 & 01 & 01 \\
01 & 02 & 03 & 01 \\
01 & 01 & 02 & 03 \\
03 & 01 & 01 & 02 \\
\end{matrix}\right]
\left[\begin{matrix}
S_{0,c} \\
S_{1,c} \\
S_{2,c} \\
S_{3,c} \\
\end{matrix}\right]
$$

其中乘法和加法都在 $\mathrm{GF}(2^8)$ 下模 $x^4+1$ 进行的。考虑对上图右边的状态进行列混合运算， 则在同一列中，一个字节会影响该列的所有字节。例如，对于上图的最后一列，字节 $S_{0,3}$ 对该列的影响为

$$
\begin{aligned}
S'_{0,3} &= S'_{0,3} \oplus (\{02\} \cdot S_{0,3}) \\
S'_{1,3} &= S'_{0,3} \oplus (\{01\} \cdot S_{0,3}) \\
S'_{2,3} &= S'_{0,3} \oplus (\{01\} \cdot S_{0,3}) \\
S'_{3,3} &= S'_{0,3} \oplus (\{03\} \cdot S_{0,3}) \\
\end{aligned}
$$

同理也可得该列其他字节对该列的影响。考虑将其应合并进 `QUARTER_ROUND_E()` 函数中，该函数将变为

```js
function QUARTER_ROUND_E(t, a, b, c, d) {
    t0 = S_Box[get_byte(t, 0)];
    set_byte(d, 0, get_byte(d, 0) ^ f2(t0));
    set_byte(d, 1, get_byte(d, 1) ^ f1(t0));
    set_byte(d, 2, get_byte(d, 2) ^ f1(t0));
    set_byte(d, 3, get_byte(d, 3) ^ f3(t0));
    // ...
}
```

其中 `f1(x)`、`f2(x)`、`f3(x)` 分别表示有限域上 `01`、`02`、`03` 与 `x` 的乘法。

可将四个字节操作合并为一个字的操作，该函数变换为

```js
function QUARTER_ROUND_E(t, a, b, c, d) {
    t0 = S_Box[get_byte(t, 0)];
    t1 = S_Box[get_byte(t, 1)];
    t2 = S_Box[get_byte(t, 2)];
    t3 = S_Box[get_byte(t, 3)];
    d ^= to_word(f3(t0), f1(t0), f1(t0), f2(t0));
    c ^= to_word(f1(t1), f1(t1), f2(t1), f3(t1));
    b ^= to_word(f1(t2), f2(t2), f3(t2), f1(t2));
    a ^= to_word(f2(t3), f3(t3), f1(t3), f1(t3));
}
```

其中 `to_word(byte b24_31, byte b16_23, byte b8_15, byte b0_7)` 表示由 4 个字节构造 1 个字。

注意到 `t0` 是字节类型，只有 256 个可能的值，所以 `to_word(f3(t0), f1(t0), f1(t0), f2(t0))` 也最多只有 256 个值，于是可以事先预处理这 $256 \times 4$ 个 32 位数。设预处理的 `Te(t) = to_word(f2(S_Box[t]), f3(S_Box[t]), f1(S_Box[t]), f1(S_Box[t]))`，`Te(t, n)` 表示将 `Te(t)` 循环右移 $8n$ 位，则 `QUARTER_ROUND_E()` 函数最终版为

```js
function QUARTER_ROUND_E(t, a, b, c, d) {
    d ^= Te(get_byte(t, 0), 3);
    c ^= Te(get_byte(t, 1), 2);
    b ^= Te(get_byte(t, 2), 1);
    a ^= Te(get_byte(t, 3), 0);
}
```

##### 合并轮密钥加

现在还剩最后一个操作未合并。考虑列混合后的状态 `(t0, t1, t2, t3)`，之后将对其进行轮密钥加操作。可直接将 `t0, t1, t2, t3` 的初值赋为该轮的轮密钥，于是一轮变换的操作就是

```js
// 旧状态为 (s0, s1, s2, s3)，新状态为 (t0, t1, t2, t3)
function OneRound(s0, s1, s2, s3, t0, t1, t2, t3, rk) {
    t0 = rk[0];
    t1 = rk[1];
    t2 = rk[2];
    t3 = rk[3];
    QUARTER_ROUND_E(s3, t0, t1, t2, t3);
    QUARTER_ROUND_E(s2, t3, t0, t1, t2);
    QUARTER_ROUND_E(s1, t2, t3, t0, t1);
    QUARTER_ROUND_E(s0, t1, t2, t3, t0);
}
```

##### 解密

## 找出生成0 0 1 0 1 0 1 0 0 1 0 0 0 1的最短线性反馈移位寄存器。
