#!/usr/bin/env python3
"""Extract combined DSP56800E WORD-addressed Motorola S-records; no flashing."""
from pathlib import Path
import sys, json, hashlib, re
from collections import Counter

def extract(src, out):
    raw=src.read_bytes(); lines=raw.decode('ascii').splitlines(); records=[]; types=Counter()
    for n,line in enumerate(lines,1):
        if not line: continue
        if not line.startswith('S'): raise ValueError(f'Line {n}: not S-record')
        typ=int(line[1]); al={0:2,1:2,2:3,3:4,5:2,6:3,7:4,8:3,9:2}[typ]
        b=bytes.fromhex(line[2:])
        if b[0]!=len(b)-1 or sum(b)%256!=255: raise ValueError(f'Line {n}: length/checksum mismatch')
        a=int.from_bytes(b[1:1+al],'big'); d=b[1+al:-1]
        records.append((typ,a,d,n)); types[typ]+=1
    if any(t in (1,2) for t,a,d,n in records): raise ValueError('Expected S3 data')
    out.mkdir(parents=True,exist_ok=True)
    report={'source':src.name,'source_sha256':hashlib.sha256(raw).hexdigest(),'record_counts':dict(types),'checksums':'all valid','address_units':'16-bit words; bytes within each word retained in original order (little endian)','entry_record_address':next(hex(a) for t,a,d,n in records if t==7),'spaces':{}}
    strings=['space\tword_address\tbyte_in_word\ttext']
    for space,offset in [('P',0),('X',0x02000000)]:
        rs=[(a-offset,d) for t,a,d,n in records if t==3 and (a>=0x02000000)==(space=='X')]
        words={}
        for a,d in rs:
            if len(d)%2: raise ValueError('Odd payload size')
            for j in range(0,len(d),2):
                addr=a+j//2; val=d[j:j+2]
                if addr in words and words[addr]!=val: raise ValueError(f'Conflicting word at {addr:x}')
                words[addr]=val
        lo=min(words); hi=max(words); buf=bytearray(b'\xff\xff'*(hi-lo+1))
        for a,val in words.items():buf[2*(a-lo):2*(a-lo)+2]=val
        spans=[]
        for a in sorted(words):
            if spans and a==spans[-1][1]+1:spans[-1][1]=a
            else:spans.append([a,a])
        fn=f'powerware_01_30_{space}_word_{lo:06X}-{hi:06X}_LE.bin'; (out/fn).write_bytes(buf)
        # Verify every original record against its output position.
        for a,d in rs: assert buf[2*(a-lo):2*(a-lo)+len(d)]==d
        report['spaces'][space]={'file':fn,'base_word_address':hex(lo),'last_word_address':hex(hi),'binary_size':len(buf),'present_bytes':2*len(words),'filler_bytes':len(buf)-2*len(words),'present_word_ranges':[[hex(a),hex(b)] for a,b in spans],'sha256':hashlib.sha256(buf).hexdigest(),'record_roundtrip':'passed'}
        for a,b in spans:
            chunk=bytes(buf[2*(a-lo):2*(b-lo+1)])
            for m in re.finditer(rb'[ -~]{5,}',chunk):
                s=m.group().decode('ascii'); strings.append(f'{space}\t0x{a+m.start()//2:06X}\t{m.start()%2}\t{s}')
    (out/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
    (out/'strings.tsv').write_text('\n'.join(strings)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__':
    extract(Path(sys.argv[1]),Path(sys.argv[2]))
