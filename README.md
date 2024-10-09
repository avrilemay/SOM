# Self-Organizing Map (SOM) in C

## Project Description

This project implements a Self-Organizing Map (SOM) algorithm in C to classify the **Wine Dataset**. SOM is an unsupervised neural network algorithm used to reduce the dimensionality of data while preserving its topological properties. In this implementation, a two-dimensional grid of neurons is trained using competitive learning to represent a high-dimensional dataset.

## Dataset

The **Wine Dataset** from UCI Machine Learning Repository was used for testing the SOM. The dataset contains results from a chemical analysis of wines grown in the same region in Italy but derived from three different grape varieties. It includes 13 attributes for 178 wine samples.

- **Number of instances:** 178
- **Attributes:** 13 (including alcohol, malic acid, ash, flavonoids, proline, etc.)
- **Classes:** 3 wine varieties

The data is preprocessed by normalizing each attribute between 0 and 1 using min-max scaling.

### Makefile

The compilation and execution process is automated via the Makefile:

*   **Compile:** `make`
*   **Run:** `make run`
*   **Clean:** `make clean`

## Program Structure

The program is designed in a modular manner for readability and maintainability. Below is a brief description of the key modules:

1.  **elements.h**  
    Defines data structures, libraries, and function declarations. It contains structures for storing the dataset, neurons, and the SOM grid.
    
2.  **main.c**  
    The main file orchestrates the execution. It initializes the dataset and SOM, triggers the training process, and displays results.
    
3.  **dataset.c**  
    Handles reading the dataset from `wine.data`, normalizes the data, and prepares it for training.
    
4.  **neural_net.c**  
    Initializes the SOM grid with neurons. Random weights are assigned to each neuron and normalized.
    
5.  **apprentissage_SOM.c**  
    Contains the learning algorithm, including the selection of the Best Matching Unit (BMU) and the update of neuron weights based on competitive learning.
    
6.  **resultats.c**  
    Displays the final classification results and calculates the accuracy of the SOM based on the dataset.
    
7.  **utils.c**  
    Provides utility functions for memory management, random number generation, and error handling.
    

## Results

After training for 1500 iterations, the SOM successfully classifies the wine dataset. The accuracy of the classification falls between 96% and 100%, which is in line with the expected performance reported in the literature for this dataset (~97.78%).

The trained SOM produces a two-dimensional map where each neuron represents a wine sample. Neurons are clustered based on the chemical properties of the wines, with neurons close to each other in the grid representing wines with similar attributes.

## SOM Classification Results

Below is an example of the SOM grid showing the classification of wine types. Each cell represents a neuron on the 10x10 SOM grid, with numbers indicating the type of wine classified by that neuron:

[1] = Cépage 1  
[2] = Cépage 2  
[3] = Cépage 3  
  
[1] [1] [1] [1] [3] [3] [3] [3] [3] [3]  
[1] [1] [1] [1] [3] [3] [3] [3] [3] [3]  
[1] [1] [1] [1] [3] [3] [3] [3] [3] [3]  
[1] [1] [1] [1] [3] [3] [3] [3] [3] [3]  
[1] [1] [1] [1] [3] [3] [3] [3] [3] [3]  
[1] [1] [1] [2] [2] [2] [2] [2] [3] [3]  
[1] [1] [1] [2] [2] [2] [2] [2] [3] [3]  
[1] [1] [1] [2] [2] [2] [2] [2] [3] [3]  
[2] [2] [2] [2] [2] [2] [2] [2] [2] [2]  
[2] [2] [2] [2] [2] [2] [2] [2] [2] [2]  

Pourcentage de justesse des catégories des vins: 97.75%


## Conclusion

This implementation demonstrates the SOM algorithm’s capability to organize and classify high-dimensional data. The modular structure of the program ensures that future modifications or improvements can be easily incorporated. The SOM successfully achieves a high classification accuracy for the Wine dataset, illustrating its effectiveness for unsupervised learning tasks.
