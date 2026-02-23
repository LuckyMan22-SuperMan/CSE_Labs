def bubble_sort(arr):
    n = len(arr)

    for i in range(n):
        swapped = False
        for j in range(n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                swapped = True

        if not swapped:  # stop if already sorted
            break

    return arr


# Take input from user
arr = list(map(int, input("Enter numbers separated by space: ").split()))

sorted_arr = bubble_sort(arr)
print("Sorted array:", sorted_arr)
