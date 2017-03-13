#! /bin/python
from math import sqrt
from os.path import isfile
from os import listdir, system
import sys

TIME_STEP = 0.01 # in seconds
NUMBER_STEP = 100

def summarize(path):
    count_file = 1
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
        if value_count > 0:
            mean /= value_count
            sd = sqrt(sd/value_count - mean**2)
            summary.append((i*TIME_STEP, mean, sd))


    #print(summary)

    file = open(path + "stats", "w")
    for (x, mean, sd) in summary:
        file.write(str(x) + " " + str(mean-2*sd) + " " + str(mean) + " " + str(mean+2*sd) + "\n")




for test_dir in listdir('output'):
    for dir in listdir('output/'+test_dir):
        summarize("output/" + test_dir + "/" + dir + "/")

