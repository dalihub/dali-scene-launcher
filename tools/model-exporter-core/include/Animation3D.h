#ifndef ANIMATION3D_H
#define ANIMATION3D_H
#include <string>
#include <vector>

struct NodeKey
{
  NodeKey()
  {
    time = v[0] = v[1] = v[2] = v[3] = 0.0f;
  }
  float time;
  //v[3] is w in quaternion and 0.0 for vector3
  float v[4];
};

class NodeAnimation3D
{
public:
  NodeAnimation3D();
  virtual ~NodeAnimation3D();

  std::string NodeName;
  std::vector<NodeKey> Rotations;
  std::vector<NodeKey> Positions;
  std::vector<NodeKey> Scales;
protected:

private:

};

class Animation3D
{
public:
  Animation3D();
  virtual ~Animation3D();
  bool HasAnimations();
  float TicksPerSecond;
  float Duration;
  std::string Name;
  std::vector<NodeAnimation3D> AnimNodesList;
protected:

private:

};

#endif // ANIMATION3D_H
