# 古典密码学作业

徐鉴劲 2015011313 计54

### Enigma 密码

#### 三转轮Enigma的构造

三个转轮由26个字母组成，每按下一个按键，最后一个转轮转一圈，后一个转轮转一整圈时，前一个转轮才转一圈。
字母按键通过三个转轮构成的映射变成密文，再通过反射器显示在灯泡上。
加密和解密的步骤是一样的：首先将转轮设置成密钥，依次键入明文（密文）。

#### 对Enigma密码的破解

1. 早期Enigma密码使用中，操作员习惯将密钥作为开头键入两遍，然后再输入正文。通过对重复的开头进行频率分析，可以有效缩小密钥空间。

2. 后期Enigma密码的使用中没有上述问题，密码学家们采用明文进行破解，

### 1.5

移位长度：16，明文：LOOKUPINTHEAIRITSABIRDITSAPLANEITSSUPERMAN。

相关实现在`decipher.cpp/ShiftCipher`中，采用的方法是穷尽密钥空间搜索，对于每一个密钥，查看其对应明文是否合理。

### 1.16

#### (a)

原置换为：

|x|1|2|3|4|5|6|7|8|
|-|-|-|-|-|-|-|-|-|
|$\pi(x)$|4|1|6|2|7|3|8|5|

逆置换为：

|x|1|2|3|4|5|6|7|8|
|-|-|-|-|-|-|-|-|-|
|$\pi^{-1}(x)$|2|4|6|1|8|3|5|7|

#### (b)

算法实现在`decipher.cpp/TBoxCipher`中。核心功能在`decipher.cpp/TBox`中，完成了根据映射进行置换的功能。

明文：GENTLEMENDONOTREADEACHOTHERSMAILGENTLEMENDONOTREADEACHOTHERSMAIL

### 1.21

#### (a)

频率分析中，单字频率前几位是：

Cipher|C|G|S|K|I|Y|
|:--|:--|:--|:--|:--|:--|:--|
Freq/% |14.45|9.38|7.81|7.03|5.86|5.47|

双字频率前几位是：

Cipher|ZC|CN|FZ|ZE|EJ|HC|
|:--|:--|:--|:--|:--|:--|:--|
Freq/% |2.75|1.96|1.57|1.18|0.78|0.39|

三字频率前几位是：

Cipher|CCN|ZEJ|KUC|
|:--|:--|:--|:--|
Freq/% |1.18|0.79|0.39|

可以用最频繁的字母E来替换C，可得部分明文为：

```
----- Cipher Compare -----

........E..DE.......D..E...........E.......E............E..DE..E..D...E....E...E
EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXEC

.E......E.......ED....E...................................EED.........ED....DE..
JCKSHYSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGY

...........E.D....D..E.....E..E..E...E..EED............E..E..EEDE..E...DE.......
EOWEUPXEZGACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZ

....E..E......E.
EGMXCYHCJUMGKUCY

----- End -----
```

1. 分析明、密文对，发现`FZCCN`重复了三次，`FZCCNDGYYSF`重复了两次，说明后者是一个单词，前者是后一个单词的前一部分。

2. `YY`是连续相同的密文对，它的明文也是成对的，而`ee`明文对已经出现，剩下在英文中可能出现的明文可能是`rr`,或者是重度音`dd`,`pp`,`tt`等。考虑`Y=R`的情况，`ER`与`RE`都是较为常见的，也就是`CY`与`YC`，前者出现了四次，后者出现了一次，不存在否定的可能。

3. `EJ`与`ZEJ`都是较为常见的部分，而且全部出现的`EJ`都是`ZEJ`的一部分，说明`ZEJ`是一个单词。

4. 由`ZC`（即`_e`)和`CN`（即`e_`）可知，它们很可能是e的常见前后缀。按照概率排序：

e作为后部：`he`,`re`,`te`,`de`,`se`,`le`,`ve`。

e作为前部：`er`,`es`,`en`,`ed`,`ea`,`et`。

首先考虑`ZC=HE`的情况，英文中`the`出现频率较高，检查长度为三的子串中，`FZC`三次，`UZC`两次，`KZC`和`SZC`一次。所以推断`F=T`，那么`FZCCN=THEED`，这是不可能的子串。那么`U=T`，进行替换，观察到结果中有如下子串：

```
HT..HEED......
ZUGFZCCNDGYYSF
```

`FZCCNDGYYSF`是一个单词，而明文中`HT`是两个单词的分界线，或者是`bought`这类单词的后缀，那么`UG=TO/A`。猜测`UG=TO`，观察到结果中有：

```
.EO..RO..O.O.
ICGIWYGKKGKGO
```

O出现的频率不合理，所以`G=A`。

5. 观察到最后一个单词

```
.A.TER
MGKUCY
```

联想到master，尝试`M=M`，`K=S`，进行替换：

```
----- Cipher Compare -----

.MA...T.EA.LET..R...L..ERS..TM..AR.E..R....ES..STASMA...EA.LEA.ES.L...ERSH.ES..E
EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXEC

.ES..R..EA....SHELS...EA..RASSASA......SA..T..A......HTA.HEEL.ARR..T.HEL....LEAR
JCKSHYSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGY

....T...HA.EAL.A.SL..E.A..RES.E.TE.THE.HEEL.ARR...T.STHE..E.HEELE..EH..LE...H..H
EOWEUPXEZGACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZ

.AM.ER.E.TMASTER
EGMXCYHCJUMGKUCY

----- End -----
```

进而观察到单词

```
SH.ES
KZSCK
```

联想到shoes，进行代换`S=O`后，观察到如下单词：

```
O.E
SOC
```

所以`O=N`，继续代换：

```
THE.HEE.
UZCFZCCN
```

由`the`可知，`.HEE.`是一个单词，联想到`WHEEL`，代换`F=W`，`N=L`:

```
----- Cipher Compare -----

.MA.NOT.EA.LETO.RO..LO.ERS..TM..AR.EN.RO...ES..STASMAN..EA.LEA.ESOL.O.ERSHOES..E
EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXEC

.ESO.RO.EAN...SHELSO..EA..RASSASAN..O..SAN.TO.A...O..HTA.HEEL.ARRO.TOHEL..N.LEAR
JCKSHYSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGY

.N..T...HA.EAL.A.SLO.E.AN.RES.E.TE.THE.HEEL.ARRO..T.STHEONE.HEELE..EH..LEO..H..H
EOWEUPXEZGACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZ

.AM.ER.E.TMASTER
EGMXCYHCJUMGKUCY

----- End -----
```

6. 目前密文中已经有很多正确子串了，说明目前的推断基本正确。继续观察到：

```
WHEEL.ARROW
FZCCNDGYYSF
```

网上搜索到`wheelbarrow`这个单词，代换`D=B`，观察到单词

```
BO..HT
DSPWZU
```

推测为`bought`，那么有代换`P=U`，`W=G`，观察到单词

```
GROW.LOWER
WYSFHNSFCY
```

所以有代换`H=F`，观察到子串：

```
BUTM.GAR.EN
DPUMLWGYICO
```

联系上文可知“花园”在此处出现概率较高，所以有：but my garden，即`L=Y`，`I=D`。

剩下的很容易推断出来，置换表见`cand_21_1.txt`。

```
----- Cipher Compare -----

IMAYNOTBEABLETOGROWFLOWERSBUTMYGARDENPRODUCES.USTASMANYDEADLEAVESOLDOVERSHOESPIE
EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXEC

CESOFROPEANDBUSHELSOFDEADGRASSASANYBODYSANDTODAYIBOUGHTAWHEELBARROWTOHELPINCLEAR
JCKSHYSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGY

INGITUPIHAVEALWAYSLOVEDANDRESPECTEDTHEWHEELBARROWITISTHEONEWHEELEDVEHICLEOFWHICH
EOWEUPXEZGACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZ

IAMPERFECTMASTER
EGMXCYHCJUMGKUCY

----- End -----
```

#### (b)

枚举长度3～100进行了Kasiski测试，结果如下：

```
--3--
MVG 156
BVF 264
DDK 198
KFT 18
KFT 156
HJV 18
HJV 138
HCT 84
RLS 18
KCG 121
AFS 60
RWX 12
VYC 42
WBB 36
BBI 36
HJV 54
JVL 54
VLN 54
LNH 54
NHI 54
HJV 12
--4--
WBBI 36
HJVL 54
JVLN 54
VLNH 54
LNHI 54
--5--
HJVLN 54
JVLNH 54
VLNHI 54
--6--
HJVLNH 54
JVLNHI 54
--7--
HJVLNHI 54
```

从结果中看出`HJVLNHI`必然是明密文对相同的，其余很多相同的明密文对都由此衍生而出。

进行了重合指数分析，调用函数`OverlapIndexAnalysis`与`VigenereAnalysis`，选择和0.065均方误差与0.065最接近的几组m：

```
0.034999 7
0.034656 8
0.030599 10
```

有上面的测试看出m=7是不可能的，m=10是最有可能的。

然后对每一个密码使用频率分析：