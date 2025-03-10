# The Accuracy of Cardinality Estimators: Unraveling the Evaluation Result Conundrum
This repository contains the source code regarding our VLDB-2025 paper submission "The Accuracy of Learned Versus Traditional Cardinality Estimators: Unraveling the Evaluation Result Conundrum"

For reproducing the evaluation results, the following steps are required:
1. Install XGBoost. The details are available at https://xgboost.readthedocs.io/en/latest/tutorials/c_api_tutorial.html
2. Clone the provided code, in $HOME for example.
3. cd $HOME/LearnedVSTraditionalCE/src/H2D/src/main
4. make
5. Run experiments using the following:
   
   cd $HOME/LearnedVSTraditionalCE/src/H2D/src/main
   
   For running experiments on dataset "longlat" in the set of datasets called "earthquake" using EXGB cardinality estimator, for example, you can run :
   
   ./main_queryset_estimates --sds earthquake --ds longlat --inDir /your/path/to/dataset --outDir your/path/for/outputfile --trainQDir /path/to/train/queries/dir --testQDir /path/to/test/queries/dir --exgb --kind -1
   
   Note : For QTS-2 the kind argument needs to be set to 2
   
   We evaluate each estimator using 1,000,000 test queries. The generated .out files contain detailed evaluation results for each specific dataset and estimator, presented as two-dimensional matrices.
   In these matrices, the rows represent the selectivity classes of the queries, while the columns correspond to q-error classes.
   The values in the cells indicate the frequency of observing a specific q-error for queries within the corresponding selectivity class.
7. After producing the output files, you can produce the aggregated results:
   
   cd $HOME/LearnedVSTraditionalCE/src/H2D/src/Compare
   
   ./main_prepare your/path/for/outputfile /path/to/aggregated/results
 
The folders [datasets](./datasets) and [testQueries](./testQueries) contain some sample datasets and test queries while our repository of 18,020 datasets and the corresponding test queries are shared on https://pi3.informatik.uni-mannheim.de/rashedi/VLDB_2025/


Note: The experiments on DeepDB (https://github.com/DataManagementLab/deepdb-public), MSCN (https://github.com/andreaskipf/learnedcardinalities), and Naru (https://github.com/naru-project/naru) have been performed using their published code


