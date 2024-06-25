import math
import random
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

def distance(point1, point2):
    return (   (point1[0]-point2[0])**2  +  (point1[1]-point2[1])**2   )**0.5

def label_correction(current_point, labels_positions):
    increments = [[0,-2],[2,0]]
    new_current_point = current_point
    tries = -1
    while(tries < len(increments)):
        nearest_point = labels_positions[0]
        for dummy_index in range(1,len(labels_positions)):
            if (distance(new_current_point, labels_positions[dummy_index]) < distance(new_current_point, nearest_point) ):
                nearest_point = labels_positions[dummy_index]
        if (distance(new_current_point, nearest_point) > 0.25 ):
            return new_current_point
        else:
            tries += 1
            new_current_point = [current_point[0]+increments[tries][0], current_point[1]+increments[tries][1]]

    return new_current_point

def draw_graph(vertices, aristas, MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT):
    plt.axis([MIN_WIDTH-2, MAX_WIDTH+2, MIN_HEIGHT-2, MAX_HEIGHT+2])
    labels_positions = [] #avoid superposing labels
    for dummy_index in range(len(vertices)):
        if dummy_index == 0 :
            plt.plot(vertices[dummy_index][0],vertices[dummy_index][1], ".", color = "lime", markersize = 12)
        else:
            plt.plot(vertices[dummy_index][0],vertices[dummy_index][1], ".", color = "black", markersize = 4)
        plt.text(vertices[dummy_index][0]+0.2, vertices[dummy_index][1]+0.1, str(dummy_index), fontsize=8)
        labels_positions.append([vertices[dummy_index][0]+0.1, vertices[dummy_index][1]+0.1])

        if (vertices[dummy_index][2] != 0 and dummy_index < 10):
        	plt.text(vertices[dummy_index][0]+1.7, vertices[dummy_index][1], str(vertices[dummy_index][2]), fontsize=4)
        elif (vertices[dummy_index][2] != 0 and dummy_index >= 10):
        	plt.text(vertices[dummy_index][0]+1.55, vertices[dummy_index][1], str(vertices[dummy_index][2]), fontsize=4)

    for dummy_index in range(len(edges)):
        point1 = (vertices[edges[dummy_index][0]][0], vertices[edges[dummy_index][0]][1])
        point2 = (vertices[edges[dummy_index][1]][0], vertices[edges[dummy_index][1]][1])
        mid_point = [0.5*(point1[0]+point2[0]), 0.5*(point1[1]+point2[1])]
        
        plt.annotate(text='', xy=point1, xytext=point2, arrowprops=dict(arrowstyle='<-', color="blue",  alpha=0.95, linewidth=0.25*random.random()+0.35))
        
        if(len("["+str(edges[dummy_index][0])+","+str(edges[dummy_index][1])+"]") >= 6):
            correct_position = label_correction([mid_point[0]-0.15,mid_point[1]], labels_positions)
            plt.text(correct_position[0]-1, correct_position[1], str(edges[dummy_index][2]), color= "brown", fontsize=8)
            plt.text(correct_position[0]-3.5, correct_position[1]-0.1, "["+str(edges[dummy_index][0])+","+str(edges[dummy_index][1])+"]" , color= "brown", fontsize=5)
            labels_positions.append([correct_position[0], correct_position[1]])
        else:
            correct_position = label_correction([mid_point[0]-0.25,mid_point[1]], labels_positions)
            plt.text(correct_position[0]-1, correct_position[1], str(edges[dummy_index][2]), color= "brown", fontsize=8)
            plt.text(correct_position[0]-3.5, correct_position[1]-0.1, "["+str(edges[dummy_index][0])+","+str(edges[dummy_index][1])+"]" , color= "brown", fontsize=5)
            labels_positions.append([correct_position[0], correct_position[1]])

        if(edges[dummy_index][3] >= 10):
            correct_position = label_correction([mid_point[0]+0.25,mid_point[1]], labels_positions)
            plt.text(correct_position[0]+1, correct_position[1], str(edges[dummy_index][3]), color= "red", fontsize=8)
            plt.text(correct_position[0]+3.25, correct_position[1]+0.1, "["+str(edges[dummy_index][0])+","+str(edges[dummy_index][1])+"]" , color= "red", fontsize=5)
            labels_positions.append([correct_position[0], correct_position[1]])
        else:
            correct_position = label_correction([mid_point[0]+0.25,mid_point[1]], labels_positions)
            plt.text(correct_position[0]+1, correct_position[1], str(edges[dummy_index][3]), color= "red", fontsize=8)
            plt.text(correct_position[0]+2.25, correct_position[1]-0.1, "["+str(edges[dummy_index][0])+","+str(edges[dummy_index][1])+"]" , color= "red", fontsize=5)
            labels_positions.append([correct_position[0], correct_position[1]])
    figure = plt.gcf() # get current figure
    plt.savefig('Solution.pdf', format='pdf')    
    plt.pause(0.00000005)   # Mini-pause before closing plot
    plt.show()

print("\nDrawing solution...")

""" Read vertices file """
vertices = []

with open("vertices_data.txt") as f:
    line = f.readline()
    MAX_WIDTH = int(line)
    line = f.readline()
    MAX_HEIGHT = int(line)
    line = f.readline()
    MIN_WIDTH  = MAX_WIDTH
    MIN_HEIGHT = MAX_HEIGHT
    while True:
        line = f.readline()
        if line == '':
            break
        row = line.split(' ')
        if ([int(row[0]), int(row[1]) ]  not in vertices):
                vertices.append( [int(row[0]), int(row[1]), int(row[2]) ] )
                if (MIN_WIDTH > int(row[0])):
                    MIN_WIDTH = int(row[0])
                if (MIN_HEIGHT > int(row[1])):
                    MIN_HEIGHT = int(row[1])
f.close()


""" Read edges file """
edges = []
with open("solution_edges_data.txt") as h:
    line = h.readline()
    while True:
        line = h.readline()
        if line == '':
            break
        row = line.split(' ')
        if ([int(row[0]), int(row[1]) ]  not in vertices):
                edges.append( [int(row[0]), int(row[1]), int(row[2]), int(row[3]) ] )
h.close()


draw_graph(vertices, edges, MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT)
