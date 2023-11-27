#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define D 5
#define K 4
#define H1 2
#define H2 3
#define H3 5
#define NUM_LAYERS 3
#define FUNC 0 // 0 for logistic 1 for relu

int neurons_per_layer[3] = {H1,H2,H3};

double generateRandomDouble() {
    
    int randInt = rand();
    double randDouble = (randInt / (double)RAND_MAX) * 2.0 - 1.0;

    return randDouble;
}

struct Neuron {
    double bias;
    double *Weights;
    double output;
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
    neuron->bias = generateRandomDouble();
    neuron->layer = layer;

    if(layer == 1){
        neuron->num_input = D;
        neuron->Weights = malloc(sizeof(double) * neuron->num_input);
    }else{
        neuron->num_input = neurons_per_layer[layer-2];
        neuron->Weights = malloc(sizeof(double) * neuron->num_input);
    }

    for(int i = 0; i < neuron->num_input; i++){
        neuron->Weights[i] = generateRandomDouble();
    }
    
}

void init_layer(struct Layer* layer, int num_neurons, int layer_no){
    layer->num_neurons = num_neurons;
    layer->neurons = malloc(sizeof(struct Neuron) * num_neurons);

    for(int i = 0; i < layer->num_neurons; i++){
        init_neurons(&layer->neurons[i], layer->layer_no + 1);
    }

}

void init_neural_network(struct Neural_Network* network, int num_layers){
    network->num_layers = num_layers;
    network->layers = malloc(sizeof(struct Layer) * num_layers);

    for(int i = 0; i < network->num_layers; i++){
        init_layer(&(network->layers[i]), neurons_per_layer[i], i);
    }

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
        }
    }
}


void forward_pass(float *x, int d, float *y, int k){
    
}

int main(int argc, char* argv[]){

    srand(time(NULL));

    struct Neural_Network network;
    init_neural_network(&network, NUM_LAYERS);

    // print the network 
    print_network(&network);

}