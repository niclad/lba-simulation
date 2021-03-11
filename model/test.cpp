#include <iostream>

#include "SimServer.h"

int main()
{
  SimServer server = SimServer(1, 0.5);

  std::cout << server << std::endl;
}