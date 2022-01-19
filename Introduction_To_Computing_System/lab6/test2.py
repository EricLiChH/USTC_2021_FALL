def hanshu_(i):
    if i==1:
        return 1
    else:
        return ((hanshu_(i-1))*10+1)
total=0
n=int(input("请输入："))
for i in range(1,n+1):
    term=(hanshu_(i))
    total+=term
print(total)