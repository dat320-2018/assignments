# A small userspace application i python for accessing the
# simp_read devicedriver

a=file('/dev/simp_read')
b=a.readline()
print 'This is what I read from the driver:', b
