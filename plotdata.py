import matplotlib.pyplot as py
import matplotlib
import sys
from ast import literal_eval


def read_from_file(filename):
	print('Reading data from {0}'.format(filename))
	sys.stdout.flush()
	data = []
	try:
		with open(filename,'r') as fl:
			for line in fl:
				data.extend(literal_eval(line.strip()))
	except IOError as e:
		print "I/O error: {0}: {1}!".format(e.errno,e.strerror)
		raise
	except:
		print "Unexpected error! ",sys.exc_info()[0]
		raise
	return data;

def generate_output_image(subplt,x_data,y_data,title,image_name):
	print 'Generating: ',image_name
	subplt.grid(True)
	plot1, = subplt.plot(x_data,[ seq[0] for seq in y_data ],'bo-',label='skip list',linewidth=1,)
	plot2, = subplt.plot(x_data,[ seq[1] for seq in y_data ],'go-',label='std::map',linewidth=1)
	subplt.legend(loc=2,fontsize='small',title=title)

def plot_input_data_image(x_data,y_data):
	py.figure()
	py.title('input data')
	py.ylabel("key value")
	py.xlabel("Number of elements")
	py.grid(True)
	plot1, = py.plot(x_data,y_data,',' ,label='data',linewidth=1,)
	py.legend(loc=2)

	#py.show()
	py.savefig("input_data.png")

try:
	test_size = read_from_file('test_size_data.txt')
	insert_time = read_from_file('insertion_data.txt')
	search_time = read_from_file('searching_data.txt')
	erase_time = read_from_file('erase_data.txt')
	input_data = read_from_file('input_data.txt')
	print 'All data ready!'
except:
	print "Operation failed! Quitting"
	sys.exit(1)

font = {'family':'serif','weight':'light','size':12}
matplotlib.rc('font', **font)

f,axarr = py.subplots(3, sharex=True)
generate_output_image(axarr[0], test_size,insert_time,"Insertion","insertion_benchmark.png")
generate_output_image(axarr[1], test_size,search_time,"Seaching","searching_benchmark.png")
generate_output_image(axarr[2], test_size,erase_time,"Erasing","erasing_benchmark.png")
axarr[0].set_title("Performance comparison (the lower then better)")
#axarr[0].legend(loc=2,fontsize='small',title='Insertion')
axarr[2].set_xlabel("Number of elements")
for ax in axarr:
	ax.set_ylabel('ms')

f.tight_layout()
f.savefig("benchmark.png")

plot_input_data_image([ seq[0] for seq in input_data ],[ seq[1] for seq in input_data ])
