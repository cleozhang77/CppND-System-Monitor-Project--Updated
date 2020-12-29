#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long prevact{0};
  long prevtotal{0};
  long curact{0};
  long curtotal{0};
};

#endif