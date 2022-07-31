import sys

current_person = None
current_count = 0
person = None

for line in sys.stdin:
    line = line.strip()
    # line: PERSON \s COUNT
    person, count = line.split(" ", 1)

    try:
        count = int(count)
    except ValueError:
        continue

    if current_person == person:
        current_count += count
    else:
        if current_person:
            print(f"{current_person} {current_count}")

        current_count = count
        current_person = person

if current_person == person:
    print(f"{current_person} {current_count}")
