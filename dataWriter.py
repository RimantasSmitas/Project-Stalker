import datetime

current_time = datetime.datetime.now()
people = 5
cyclist = 2
year = current_time.year
month = current_time.month
day = current_time.day
hour = current_time.hour
minute = current_time.minute

f = open("dataFile.txt","a")

peeps = "{0}{1}{2}{3}{4} {5}\n"
f.write(peeps.format(year,month,day,hour,minute,people))

bikes = "{0}{1}{2}{3}{4} {5}\n"
f.write(peeps.format(year,month,day,hour,minute,cyclist))

f.close()
###########
#### Reading the settings from a file ####
###########
### the format in the file will as follows theres going a setting per line
### first line coordinates of the counting line
### second line frame rate and resolution setting a number from 1 to 3
### third line DEV EUI
### fourth line APP EUI
### fifth line APP KEY
### For the python side of the software we need to read just the first two lines the rest are for C
s = open("settings.txt","r+")
coordinates = (s.readline())
recordingSetting = (s.readline())
s.close()

print(coordinates)
print(recordingSetting)
