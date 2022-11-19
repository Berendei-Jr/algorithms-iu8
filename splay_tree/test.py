try:
    raise KeyError()
except KeyError:
    print("catch")    