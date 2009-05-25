#!/bin/python

import os, sys, time

lines = open(sys.argv[1], 'rb').readlines()

# logfile format:
# <time(ms)> <key>: <value>
# example:
# 16434 read cache: 17

key_order = ['receive buffer', 'send buffer', 'write cache', 'read cache', 'hash temp']
colors = ['30f030', 'f03030', '80f080', 'f08080', '4040ff']

keys = []
fields = {}
maximum = {}
out = open('disk_buffer_log.dat', 'w+')

field_sum = {}
field_num_samples = {}

for c in key_order:
	keys.append(c)
	fields[c] = 0
	maximum[c] = 0
	field_sum[c] = 0
	field_num_samples[c] = 0

last_t = 0
for l in lines:
	try:
		t = int(l[0:l.find(' ')])
		c = l[l.find(' ')+1:l.find(':')]
		n = int(l[l.find(':')+1:-1])
	except:
		print l
		continue

	if last_t != t:
		print >>out, '%d\t' % last_t,
		for i in keys:
			print >>out, '%d\t' % maximum[i],
		print >>out, '\n',

	if not c in keys:
		keys.append(c)
		fields[c] = 0
		maximum[c] = 0

	fields[c] = n

	field_sum[c] += n
	field_num_samples[c] += 1

	if n > maximum[c]: maximum[c] = n

	if last_t != t:
		last_t = t
		maximum = fields

for i in keys:
	print '%s: avg: %f' % (i, float(field_sum[i]) / float(field_num_samples[i]))
print
	
out.close()

out = open('disk_buffer.gnuplot', 'wb')
print >>out, "set term png size 1200,700"
print >>out, 'set output "disk_buffer.png"'
print >>out, 'set xrange [0:*]'
print >>out, 'set xlabel "time (ms)"'
print >>out, 'set ylabel "buffers"'
print >>out, "set style data lines"
print >>out, "set key box"
print >>out, 'plot',
count = 1 + len(keys)
keys.reverse()
for k in keys:
	expr = "$%d" % count
	for i in xrange(2, count): expr += "+$%d" % i
	count -= 1
	print >>out, ' "disk_buffer_log.dat" using 1:(%s) title "%s" with filledcurves x1 lt rgb "#%s",' % (expr, k, colors[count-1]),
print >>out, 'x=0'
out.close()

os.system('gnuplot disk_buffer.gnuplot')
