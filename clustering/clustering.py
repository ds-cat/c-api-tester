from sklearn.datasets import make_blobs
from sklearn.cluster import DBSCAN
from sklearn.preprocessing import StandardScaler
from optparse import OptionParser
import json
from scipy import spatial
import numpy as np
import gensim
from gensim.models import Word2Vec
import time
import matplotlib.pyplot as plt
import csv
import nltk
from nltk.cluster import KMeansClusterer
from nltk.cluster import euclidean_distance
from nltk import cluster
from numpy import array
sentence_vectors_unprocessed = []

#loop opens csv and converts it into list of sentacne vectors in format[[list of some numbers that represent the sentances],[],...,[]]
with open('vectors_save.csv', newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in spamreader:
        temp2 = []
        temp = str(row)
        temp = temp.replace("['", '')
        temp = temp.replace("']", '')
        
        #print(temp)
        temp = (temp.split(','))

        for x in temp:
            #print(x)
            try:
                temp2.append(float(x))
            except:
                #print("empty value occured")
                #do anything
                waster = 1
        #print(temp2[1])


        sentence_vectors_unprocessed.append(temp2)

j = 0
def top_bottom_avg(lst):
    if(lst == []):
        return
    # Sort the list in ascending order
    sorted_lst = sorted(lst)
    
    # Get the length of the list
    n = len(sorted_lst)
    
    # Calculate the midpoint of the list
    midpoint = n // 2
    
    # Calculate the average of the top half
    top_half = sorted_lst[midpoint:]
    top_half_avg = sum(top_half) / len(top_half)
    
    # Calculate the average of the bottom half
    bottom_half = sorted_lst[:midpoint]
    bottom_half_avg = sum(bottom_half) / len(bottom_half)
    
    # Return the averages in the format [x, y]
    return [bottom_half_avg, top_half_avg]

sentence_vectors_processed = []
reviews = open("reviews.txt", "r", encoding="utf-8")
sentences = reviews.readlines()
for c in sentence_vectors_unprocessed:
    if(j%2 == 0 ):
        print(j)
        sentence_vectors_processed.append(top_bottom_avg( sentence_vectors_unprocessed[j]))
    j = j+1
#for index, sentence in enumerate(sentences):    
  #  print (str(assigned_clusters[index]) + ":" + str(sentence))
centers =  [array(f) for f in sentence_vectors_processed]
X, labels_true = make_blobs(n_samples=3000, centers=centers, cluster_std=0.7)

import time
from sklearn.cluster import KMeans
cluster_size = 24
k_means = KMeans(init="k-means++", n_clusters=cluster_size, n_init=10)
t0 = time.time()
k_means.fit(X)
t_batch = time.time() - t0
print(k_means.labels_[0])
index = 0
with open('clustered_sentances_with_invocation_' +str(cluster_size)+'.csv', 'w', newline='',encoding="utf-8") as f:
    writer = csv.writer(f)
    while(index < 2000):
        output = [sentences[index], k_means.labels_[index]]
        writer.writerow(output)
        index = index + 1
    f.close()



