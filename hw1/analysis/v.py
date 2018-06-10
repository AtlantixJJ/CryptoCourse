import pprint
import collections

c_str = [l.strip() for l in open("cipher/segment.txt").readlines()]

TBox = {}

def frequency_analysis(cs, LEN=1):
    dic = collections.OrderedDict()

    for i in range(len(cs)):
        if i + LEN >= len(cs):
            break
        
        c = cs[i:i+LEN]

        try:
            dic[c] += 1.0
        except:
            dic[c] = 1.0
    
    cnt = len(cs) - LEN + 1

    for k in dic.keys():
        dic[k] /= cnt

    return dic

def print_first(inv_dic, keys):
    cnt = 0
    s = ""
    for key in keys:
        print("%.5f %s" % (key * 100, inv_dic[key]))
        s += inv_dic[key]
    return s


s = ""
for i,l in enumerate(c_str):
    dic = frequency_analysis(l)
    inv_dic = {v: k for k, v in dic.items()}
    vals = inv_dic.keys(); vals.sort()
    c = print_first(inv_dic, vals[-1:])
    c = chr((26 + ord(c) - ord('E'))%26+ord('A'))
    s += c
print(s)