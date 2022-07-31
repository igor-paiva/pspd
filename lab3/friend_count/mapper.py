import sys

for line in sys.stdin:
    line = line.strip()
    words = line.split(", ")
    person = words.pop(0)

    print(f"{person} {len(words)}")
