// extern "C" void C_library_function(int x, int y);
#include "/Users/davidfreifeld/Downloads/eigen-3.3.7/Eigen/Dense"
#include <vector>
#include <array>
#include <iostream>
#include <cstdio>

class Node;
class Edge {
public:
  Node* source;
  Node* end;
  int weight;

  Edge(Node* srcaddr, Node* endaddr);
};

Edge::Edge(Node* srcaddr, Node* endaddr)
{
  source = srcaddr;
  end = endaddr;
  weight = rand();
}

class Node {
public:
  std::vector<Edge> incoming;
  std::vector<Edge> outgoing;
  int activation;
  int bias;

  Node();
};

Node::Node()
{
  activation = 0;
  bias = rand();
}

class Layer {
public:
  Eigen::MatrixXd* contents;
  Eigen::MatrixXd* weights;

  Layer(float* vals, int rows, int columns);
  Layer(int rows, int columns);
  void initWeights(Layer next);
};

Layer::Layer(float* vals, int batch_sz, int nodes)
{
  contents = new Eigen::MatrixXd (batch_sz, nodes);
  int datalen = batch_sz*nodes;
  for (int i = 0; i < datalen; i++) {
    (*contents)((int)i / nodes,i%nodes) = vals[i];
  }
  std::cout << *contents << "\n\n";
}

Layer::Layer(int batch_sz, int nodes)
{
  contents = new Eigen::MatrixXd (batch_sz, nodes);
  int datalen = batch_sz*nodes;
  for (int i = 0; i < datalen; i++) {
    (*contents)((int)i / nodes,i%nodes) = 0;
  }
  std::cout << *contents << "\n\n";
}

void Layer::initWeights(Layer next)
{
  weights = new Eigen::MatrixXd (contents->cols(), next.contents->cols());
  printf("%i x %i\n", weights->rows(), weights->cols());
  int nodes = weights->cols();
  for (int i = 0; i < (weights->rows()*weights->cols()); i++) {
    (*weights)((int)i / nodes, i%nodes) = rand() / double(RAND_MAX);
  }
  std::cout << *weights << "\n\n";
}

class Network {
public:
  std::vector<Layer> layers;
  int length;

  Network(char* path, int inputs, int hidden, int outputs, int neurons, int batch_sz);
  void feedforward();
  void activate(Eigen::MatrixXd matrix);
};

Network::Network(char* path, int inputs, int hidden, int outputs, int neurons, int batch_sz)
{
  length = hidden + 2;
  FILE* fptr = fopen(path, "r");
  int datalen = batch_sz*inputs;
  float batch[datalen];
  char line[1024] = {' '};
  for (int i = 0; i < batch_sz; i++) {
    fgets(line, 1024, fptr);
    sscanf(line, "%f,%f,%f,%f,*f", &batch[0+(i*inputs)], &batch[1+(i*inputs)], &batch[2+(i*inputs)], &batch[3+(i*inputs)]);
  }
  float* batchptr = batch;
  layers.emplace_back(batchptr, batch_sz, inputs);
  for (int i = 0; i < hidden; i++) {
    layers.emplace_back(batch_sz, neurons);
  }
  layers.emplace_back(batch_sz, outputs);
  for (int i = 0; i < hidden+1; i++) {
    layers[i].initWeights(layers[i+1]);
  }
}

void Network::activate(Eigen::MatrixXd matrix)
{
  int nodes = matrix.cols();
  for (int i = 0; i < (matrix.rows()*matrix.cols()); i++) {
    // (matrix)((int)i / nodes, i%nodes) = 
  }
}

void Network::feedforward()
{
  for (int i = 0; i < length-1; i++) {
    std::cout << "-----------\nRUN\n\n" << *layers[i].contents << "\n-\n\n";
    Eigen::MatrixXd product = (*layers[i].contents) * (*layers[i].weights);
    activate(product);
    std::cout << "NEXT\n\n\n" << *layers[i+1].contents << "\n-\n\n";
    layers[i+1].contents = &product;
    std::cout << "UPDATE\n\n\n" << *layers[i+1].contents << "\n\n";
  }
}


int main()
{
  Network net ("./data_banknote_authentication.txt", 4, 2, 2, 5, 10);
  net.feedforward();
}
