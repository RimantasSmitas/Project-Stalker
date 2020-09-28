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
