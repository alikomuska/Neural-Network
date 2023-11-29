#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define D 2 // number of inputs
#define K 4 // number of outputs
#define H1 6 // hidden layer 1
#define H2 3 // ... 2
#define H3 4 // ... 3
#define NUM_LAYERS 3
#define FUNC 0 // 0 for logistic 1 for hyberbolic 2 for relu


// 3 hidden layers and one output layer
int neurons_per_layer[4] = {H1,H2,H3, K};

float generateRandomfloat() {
    
    int randInt = rand();
    float randfloat = (randInt / (float)RAND_MAX) * 2.0 - 1.0;

    return randfloat;
}

float sigmoid(float x) {

    float result = 1.0 / (1.0 + exp(-x));

    return result;
}

float relu(float x) {

    float result = fmax(0, x);

    return result;
}

float hyperbolic(float x) {

    float result = (exp(x) - exp(-x)) / (exp(x) + exp(-x));

    return result;
}

float linear(float x) {

    return x;
}


struct Neuron {
    float bias;
    float *Weights;
    float output;
    int layer;
    int num_input;

};

struct Layer {
    int num_neurons;
    int layer_no;
    struct Neuron *neurons;
};

struct Neural_Network {
    int num_layers;
    struct Layer *layers;
};

void init_neurons(struct Neuron* neuron, int layer){
    neuron->bias = generateRandomfloat();
    neuron->layer = layer;

    //printf("MY LAYER IS %d\n", layer);
    

    if(layer == 1){
        neuron->num_input = D;
        neuron->Weights = malloc(sizeof(float) * neuron->num_input);
    }else{
        neuron->num_input = neurons_per_layer[layer-2];
        neuron->Weights = malloc(sizeof(float) * neuron->num_input);
    }

    for(int i = 0; i < neuron->num_input; i++){
        neuron->Weights[i] = generateRandomfloat();
    }
    
}

void init_layer(struct Layer* layer, int num_neurons, int layer_no){
    layer->num_neurons = num_neurons;
    layer->layer_no = layer_no + 1;
    layer->neurons = malloc(sizeof(struct Neuron) * num_neurons);

    for(int i = 0; i < layer->num_neurons; i++){
        //printf("Layer num %d\n", layer_no);
        init_neurons(&layer->neurons[i], layer->layer_no);
    }

}

void init_neural_network(struct Neural_Network* network, int num_layers){
    network->num_layers = num_layers + 1;
    network->layers = malloc(sizeof(struct Layer) * network->num_layers);
    
    for(int i = 0; i < num_layers; i++){
        init_layer(&(network->layers[i]), neurons_per_layer[i], i);
    }

    init_layer(&(network->layers[num_layers]), neurons_per_layer[num_layers], num_layers);

}

void print_network(struct Neural_Network* network) {
    printf("Neural Network Architecture:\n");

    for (int i = 0; i < network->num_layers; i++) {
        printf("\nLayer %d:\n", i + 1);
        printf("Number of neurons: %d\n", network->layers[i].num_neurons);

        for (int j = 0; j < network->layers[i].num_neurons; j++) {
            printf("\nNeuron %d:\n", j + 1);
            printf("Bias: %lf\n", network->layers[i].neurons[j].bias);
            printf("Number of input neurons: %d\n", network->layers[i].neurons[j].num_input);

            printf("Weights:\n");
            for (int k = 0; k < network->layers[i].neurons[j].num_input; k++) {
                printf("  Weight %d: %lf\n", k + 1, network->layers[i].neurons[j].Weights[k]);
            }

            printf("print the output %lf \n", network->layers[i].neurons[j].output);
        }
    }
    printf("\n");
}


void forward_pass(struct Neural_Network* network, float *x, int d, float *y, int k){
    int i = 0;
    float *input_vector = NULL;
    float *output_vector = NULL;
    
    for (i; i < NUM_LAYERS + 1; i++) {
        
        if (i == 0) {
            // If we're in the first hidden layer, the input vector is the x vector
            input_vector = malloc(sizeof(float) * d);
            for (int l = 0; l < d; l++) {
                input_vector[l] = x[l];
            }
        } else {
            // Free the output_vector from the previous layer
            free(output_vector);

            // Free the output_vector from the previous layer
            free(input_vector);
            
            // Allocate memory for the input_vector of the current layer
            input_vector = malloc(sizeof(float) * network->layers[i-1].num_neurons);
            
            // Copy values from the output_vector of the previous layer to the input_vector
            for (int l = 0; l < network->layers[i-1].num_neurons; l++) {
                input_vector[l] = network->layers[i-1].neurons[l].output;
            }
        }
        
        // Allocate memory for the output_vector of the current layer
        output_vector = malloc(sizeof(float) * network->layers[i].num_neurons);
        
        for (int j = 0; j < network->layers[i].num_neurons; j++) {
            float weighted_sum = 0;
            for (int k = 0; k < network->layers[i].neurons[j].num_input; k++) {
                weighted_sum += input_vector[k] * network->layers[i].neurons[j].Weights[k]; 
            }
            weighted_sum += network->layers[i].neurons[j].bias;

            if(i == NUM_LAYERS) {
                // linear for output layer
                network->layers[i].neurons[j].output = linear(weighted_sum);
            }else {
                // hidden layers activation functions
                if (FUNC == 0) {
                    // Logistic (Sigmoid) Activation Function
                    network->layers[i].neurons[j].output = sigmoid(weighted_sum);
                } else if(FUNC == 1) {
                    // hyperbolic Activation Function
                    network->layers[i].neurons[j].output = hyperbolic(weighted_sum);
                } else {
                    // relu
                    network->layers[i].neurons[j].output = relu(weighted_sum);
                }
            }
            // Copy the values of each neuron to the output vector 
            output_vector[j] = network->layers[i].neurons[j].output;
        }
    }

    // Calculate output layer vector y with k
    for (int l = 0; l < k; l++) {
        y[l] = output_vector[l]; 
    }

    // Free memory allocated for input and output vectors
    free(input_vector);
    free(output_vector);
}


int main(int argc, char* argv[]) {
    srand(time(NULL));

    struct Neural_Network network;
    init_neural_network(&network, NUM_LAYERS);

    // Allocate memory for input and output vectors
    float input[D] = {0.5, -0.7};  // Example input values in the range [-1, 1]
    float *output = malloc(sizeof(float) * K);

    // Perform the forward pass
    forward_pass(&network, input, D, output, K);

    
    // Print the output vector
    printf("Output Vector:\n");
    for (int i = 0; i < K; i++) {
        printf("%f ", output[i]);
    }
    printf("\n");

    // Free allocated memory
    free(output);

    print_network(&network);

    return 0;
} 

