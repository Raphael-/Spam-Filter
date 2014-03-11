#include "instancepool.h"
#include "knnclassifier.h"
#include "nbclassifier.h"
#include "baselineclassifier.h"
#include "classifierevaluator.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void printResults(const string& headerMessage, const ClassifierEvaluator& evaluator) {
   cout << headerMessage << endl
        << "accuracy: " << evaluator.getAccuracy() << endl
        << "precision: " << evaluator.getPrecision() << endl
        << "recall: " << evaluator.getRecall() << endl;
}

int main() {
   ifstream trainingFile("training_data.txt");
   ifstream testFile("test_data.txt");
   InstancePool trainingPool, testPool;
   trainingFile >> trainingPool;
   testFile >> testPool;

   KNNClassifier kNN;
   ClassifierEvaluator kNNEvaluator(kNN, trainingPool, testPool);
   printResults("kNN evaluation:", kNNEvaluator);

   NaiveBayesClassifier nb;
   ClassifierEvaluator nbEvaluator(nb, trainingPool, testPool);
   printResults("Naive Bayes evaluation:", nbEvaluator);

   BaselineClassifier base;
   ClassifierEvaluator baseEvaluator(base, trainingPool, testPool);
   printResults("Baseline evaluation:", baseEvaluator);
}
