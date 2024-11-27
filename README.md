# The Accuracy of Learned Versus Traditional Cardinality Estimators: Unraveling the Evaluation Result Conundrum
This repository contains the source code regarding our VLDB-2025 paper submission "The Accuracy of Learned Versus Traditional Cardinality Estimators: Unraveling the Evaluation Result Conundrum"
To use the code for reproducing the evaluation results, the following steps are required:
1. Install XGBoost. The details are available at https://xgboost.readthedocs.io/en/latest/tutorials/c_api_tutorial.html
2. Clone the provided code, in $HOME for example.
3. cd $HOME/LearnedVSTraditionalCE/src/H2D/src/main
4. make
5. Run experiments using the following:
   
   cd $HOME/LearnedVSTraditionalCE/src/H2D/src/main
   
   For running experiments on dataset "longlat" in the set of datasets called "earthquake" using EXGB cardinality estimator, for example, you can run :
   
   ./main_queryset_estimates --sds earthquake --ds longlat --inDir /your/path/to/dataset --outDir your/path/for/outputfile --trainQDir /path/to/train/queries/dir --testQDir /path/to/test/queries/dir --exgb --kind -1
   
   For QTS-2 the kind argument needs to be set to 2
   
   We evaluate each estimator using 1,000,000 test queries. The generated .out files contain detailed evaluation results for each specific dataset and estimator, presented as two-dimensional matrices.
   In these matrices, the rows represent the selectivity classes of the queries, while the columns correspond to q-error classes.
   The values in the cells indicate the frequency of observing a specific q-error for queries within the corresponding selectivity class.
7. After producing the output files, you can produce the aggregated results:
   
   cd $HOME/LearnedVSTraditionalCE/src/H2D/src/Compare
   
   ./main_prepare your/path/for/outputfile /path/to/aggregated/results
 
[cc-sc-esr-t1-qc-cnt-5datasets.rel](./cc-sc-esr-t1-qc-cnt-5datasets.rel) includes the aggregated evaluation results on the 5 datasets in the subset S1 introduced in the paper. 

Rank.py contains our python code for ranking the estimators.

dominace.py contains our python code for creating the dominance matrix and the corresponding visualization for pair-wise comparison of the estimators.

Note: The experiments on MSCN (https://github.com/andreaskipf/learnedcardinalities) and Naru (https://github.com/naru-project/naru) have been performed using their published code 
