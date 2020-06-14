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
  Eigen::MatrixXd* bias;

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
  bias = new Eigen::MatrixXd (1, nodes);
  for (int i = 0; i < nodes; i++) {
    (*bias)(0,i) = 0.001;
  }
}

Layer::Layer(int batch_sz, int nodes)
{
  contents = new Eigen::MatrixXd (batch_sz, nodes);
  int datalen = batch_sz*nodes;
  for (int i = 0; i < datalen; i++) {
    (*contents)((int)i / nodes,i%nodes) = 0;
  }
  bias = new Eigen::MatrixXd (1, nodes);
  for (int i = 0; i < nodes; i++) {
    (*bias)(0,i) = 0.001;
  }
}

void Layer::initWeights(Layer next)
{
  weights = new Eigen::MatrixXd (contents->cols(), next.contents->cols());
  int nodes = weights->cols();
  for (int i = 0; i < (weights->rows()*weights->cols()); i++) {
    (*weights)((int)i / nodes, i%nodes) = rand() / double(RAND_MAX);
  }
}

class Network {
public:
  std::vector<Layer> layers;
  int length;

  int batch_size;
  Eigen::MatrixXd* labels;

  Network(char* path, int inputs, int hidden, int outputs, int neurons, int batch_sz);

  Eigen::MatrixXd activate(Eigen::MatrixXd matrix);
  void feedforward();
  void list_net();

  float cost();
  float gradient(int mode, int layer, int node);
  void backpropagate();
};

Network::Network(char* path, int inputs, int hidden, int outputs, int neurons, int batch_sz)
{
  length = hidden + 2;
  batch_size = batch_sz;
  FILE* fptr = fopen(path, "r");
  int datalen = batch_sz*inputs;
  float batch[datalen];
  labels = new Eigen::MatrixXd (batch_sz, 1);
  int label;
  char line[1024] = {' '};
  for (int i = 0; i < batch_sz; i++) {
    fgets(line, 1024, fptr);
    sscanf(line, "%f,%f,%f,%f,%i", &batch[0+(i*inputs)], &batch[1+(i*inputs)], &batch[2+(i*inputs)], &batch[3+(i*inputs)], &label);
    (*labels)(i,0) = label;
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

Eigen::MatrixXd Network::activate(Eigen::MatrixXd matrix)
{
  int nodes = matrix.cols();
  for (int i = 0; i < (matrix.rows()*matrix.cols()); i++) {
    // if ((matrix)((float)i / nodes, i%nodes) < 0) {
    //   (matrix)((float)i / nodes, i%nodes) = 0;
    // }
  }
  return matrix;
}

void Network::feedforward()
{
  for (int i = 0; i < length-1; i++) {
    *layers[i+1].contents = (*layers[i].contents) * (*layers[i].weights);
    for (int j = 0; j < layers[i+1].contents->rows(); j++) {
      // layers[i+1].contents->row(j) += *layers[i+1].bias;
    }
    *layers[i+1].contents = activate(*layers[i+1].contents);
  }
}

void Network::list_net()
{
  for (int i = 0; i < length-1; i++) {
    std::cout << " LAYER " << i << "\n\n" << *layers[i].contents << "\n\n AND BIAS\n" << *layers[i].bias << "\n\n W/ WEIGHTS \n" << *layers[i].weights << "\n\n\n";
  }
  std::cout << " LAYER " << length-1 << "\n\n" << *layers[length-1].contents << "\n\n AND BIAS\n" << *layers[length-1].bias <<  "\n\n\n";
}

float Network::cost()
{
  float sum = 0;
  for (int i = 0; i < layers[length-1].contents->rows(); i++) {
    sum += pow((*labels)(i, 0) - (*layers[length-1].contents)(i, 0),2);
  }
  return (1.0/batch_size) * sum;
}

float Network::gradient(int mode, int layer, int node)
{
  // float N = batch_size;
  // if (mode == 0) {
  //   for (int i = 0; i < N; i++) {
  //     double label = labels[i];
  //     double x_i = (layers[layer].contents->row(i) / layers[layer-1].weights->col(i))(0,0) - label;
  //     std::
  //     // e_i() = e_i - label;
  //     // Eigen::MatrixXd w_i = layers[layer-1].weights->col(i);
  //     // Eigen::MatrixXd b = layers[layer].bias;
  //     // if (w_i.dot(x_i) + b)
  //   }
  // }
}

void Network::backpropagate()
{
  std::vector<Eigen::MatrixXd> gradients;
  std::vector<Eigen::MatrixXd> deltas;
  deltas.push_back(((*layers[length-1].contents - *labels).array() * layers[length-1].contents->array()).matrix());
  std::cout << deltas[0].matrix() << " \n\n\n " << layers[length-2].contents->transpose() << "\n\n\n\n" << deltas[0].matrix().dot(deltas[0]);





  // std::cout << deltas[0];
  for (int i = length-2; i >= 0; i--) {

  }
}

int main()
{
  std::cout << "\n\n\n";
  Network net ("./data_banknote_authentication.txt", 4, 2, 1, 5, 10);
  net.feedforward();
  net.list_net();
  net.backpropagate();
  std::cout << "\nCOST: " << net.cost() << "\n";
}
