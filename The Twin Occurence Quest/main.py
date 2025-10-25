# a simple parser for python. use get_number() and get_word() to read
def parser():
    while 1:
        data = list(input().split(' '))
        for number in data:
            if len(number) > 0:
                yield(number)   

input_parser = parser()

def get_word():
    global input_parser
    return next(input_parser)

def get_number():
    data = get_word()
    try:
        return int(data)
    except ValueError:
        return float(data)

arr_length = get_number()
num_searches = get_number()

arr = []

for i in range(arr_length):
    element = get_number()
    arr.append(element)

def find_first_occurrence(arr, x):
    left, right = 0, len(arr) - 1
    result = -1
    
    while left <= right:
        mid = (left + right) // 2
        
        if arr[mid] == x:
            # continue
            result = mid
            right = mid - 1
        elif arr[mid] < x:
            left = mid + 1
        else:
            right = mid - 1
    
    return result

def find_last_occurrence(arr, x):
    left, right = 0, len(arr) - 1
    result = -1
    
    while left <= right:
        mid = (left + right) // 2
        
        if arr[mid] == x:
            # continue searching right, we are looking for the last occurence
            result = mid
            left = mid + 1
        elif arr[mid] < x:
            left = mid + 1
        else:
            right = mid - 1
    
    return result

for _ in range(num_searches):
    x = get_number()
    
    first = find_first_occurrence(arr, x)
    
    if first == -1:
        print("-1 -1")
    else:
        last = find_last_occurrence(arr, x)
        print(first + 1, last + 1)
