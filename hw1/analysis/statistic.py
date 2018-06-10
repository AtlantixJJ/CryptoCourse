import pprint
import collections

c_str = open("cipher/21_2.txt").read().replace("\n", "").replace("\r", "")

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
    for key in keys:
        print("%.5f %s" % (key * 100, inv_dic[key]))

for i in range(1, 4, 1):
    dic = frequency_analysis(c_str, i)
    inv_dic = {v: k for k, v in dic.items()}
    vals = inv_dic.keys(); vals.sort()
    print(inv_dic)
    print_first(inv_dic, vals[-10:]); print("\n---")
