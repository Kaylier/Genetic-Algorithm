#! /bin/python
from math import sqrt
from os.path import isfile
from os import listdir, system
import sys

NUMBER_STEP = 30
TIME_STEP = 1.0/NUMBER_STEP # in seconds

def summarize(path):
    count_file = 0
    while (isfile(path + str(count_file))):
        count_file += 1

    print(path + " (" + str(count_file) + " files found)")

    values = [[] for i in range(NUMBER_STEP)]

    for filename in [path + str(i) for i in range(count_file)]:
        with open(filename, "r") as file:
            index = 0
            current_time_upper_bound = TIME_STEP

            sum_x = 0
            sum_y = 0
            sum_xx = 0
            sum_xy = 0
            sum_count = 0

            for line in file:
                [x,y] = [float(c) for c in line.split()]
                if x > current_time_upper_bound:
                    if sum_count > 1:
                        # Compute linear regression and add the result
                        beta = (sum_count*sum_xy - sum_x*sum_y) / (sum_count*sum_xx - sum_x*sum_x)
                        alpha = sum_y/sum_count - beta*sum_x/sum_count
                        values[index].append(alpha);
                    else:
                        values[index].append(y);
                    # Update timezone
                    current_time_upper_bound += TIME_STEP
                    index += 1
                    if index >= NUMBER_STEP:
                        break;
                    # Reinitialize linear regression for the next timezone
                    sum_x = 0
                    sum_y = 0
                    sum_xx = 0
                    sum_xy = 0
                    sum_count = 0
                sum_x += x - index*TIME_STEP
                sum_y += y
                sum_xx += (x-index*TIME_STEP)**2
                sum_xy += (x-index*TIME_STEP)*y
                sum_count += 1

    #print(values)

    summary = [];
    for i in range(NUMBER_STEP):
        mean = 0
        sd = 0
        value_count = 0
        for y in values[i]:
            mean += y
            sd += y*y
            value_count += 1
        if value_count > 1:
            mean /= value_count
            # When the values are very close, the difference can be negative, 
            if (sd/value_count - mean**2 <= 0.):
                sd = 0.
            else:
                sd = sqrt(sd/value_count - mean**2)
            summary.append((i*TIME_STEP, mean, sd))


    #print(summary)

    if (count_file > 0):
        file = open(path + "stats", "w")
        for (x, mean, sd) in summary:
            file.write(str(x) + " " + str(mean-2*sd) + \
                                " " + str(mean-sd) + \
                                " " + str(mean) + \
                                " " + str(mean+sd) + \
                                " " + str(mean+2*sd) + \
                                "\n")
        file.close()


def compare(path_ref, path_test):
    better_test = 0.
    worse_test = 0.
    with open(path_ref + "stats", "r") as fileref, open(path_test + "stats", "r") as filetest:
        next(fileref)
        next(filetest)
        for l1,l2 in zip(fileref, filetest):
            [x1,a1,b1,m1,c1,d1] = [float(c) for c in l1.split()]
            [x2,a2,b2,m2,c2,d2] = [float(c) for c in l2.split()]
            if (a1 > m2):
                better_test = better_test + a1 - m2
            if (a2 > m1):
                worse_test = worse_test + a2 - m1
    print(path_test + ":" + str(better_test) + " " + str(worse_test))
    if (better_test > 15. and worse_test < 1.):
        return 1
    if (worse_test > 15. and better_test < 1.):
        return -1
    return 0



for instance in listdir('output'):
    for test in listdir('output/'+instance):
        for value in listdir('output/'+instance+'/'+test):
            path = "output/" + instance + "/" + test + "/" + value + "/";
            summarize(path)
            
outputfile = open("plotscript", "w");

for instance in listdir('output'):
    path_ref = "output/" + instance + "/no-redundancy/0/"
    for test in listdir('output/'+instance):
        outputfile.write("set title \""+instance+"\"\n")
        for value in listdir('output/'+instance+'/'+test):
            path_test = "output/" + instance + "/" + test + "/" + value + "/";
            if (isfile(path_test + "stats")):

                compare(path_ref, path_test)

                outputfile.write("plot \"" + path_ref + "stats\"" +\
                        " using 1:4 with lines linetype rgb \"#000000ff" + \
                        "\" title ''\n")
                outputfile.write("replot \"" + path_ref + "stats\"" + \
                        " using 1:2:6 with filledcurves closed " + \
                        "linetype rgb \"#880000ff" + "\" title \"no redundancy\"\n")
                outputfile.write("replot \"" + path_test + "stats\"" +\
                        " using 1:4 with lines linetype rgb \"#00ff0000" + \
                        "\" title ''\n")
                outputfile.write("replot \"" + path_test + "stats\"" + \
                        " using 1:2:6 with filledcurves closed " + \
                        "linetype rgb \"#88ff0000" + "\" title \"" + test + " " + value + "\"\n")
                outputfile.write("pause -1\n")


outputfile.close()
