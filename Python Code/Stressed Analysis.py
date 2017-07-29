# -*- coding: utf-8 -*-
"""
Created on Thu Apr 27 13:13:37 2017

@author: PG
"""

import sklearn
from sklearn import tree
import pandas as pd
import os

os.chdir("Datasets")

dftrain = pd.read_csv("training.csv", sep = ',', header = None)
print(dftrain)

trainlab = dftrain[0]
trainftr = dftrain.ix[:,1:]

clf = tree.DecisionTreeClassifier()
clf = clf.fit(trainftr,trainlab)
print("\n Succesfully Trained. Let us begin testing!!!")

dftest = pd.read_csv("testing.csv", sep = ',', header = None)
testlab = dftest[0]
testftr = dftest.ix[:,1:]

res = clf.predict(testftr.ix[:,:])
print(res)

ver = pd.DataFrame({"testing_set_values":testlab, "result_values":res})
print(ver)

print("No of samples Matched : " + str(ver.result_values[ver.testing_set_values==ver.result_values].count()))
print("Accuracy : " +str(float((sklearn.metrics.accuracy_score(testlab, res, normalize=True, sample_weight=None))*100)))