#include "ResourceHanger.h"
#include <vector>
#include "Singleton.h"
#include "Sprite.h"

class ResourceHanger : public Singleton<ResourceHanger> {
	friend Singleton<ResourceHanger>;

private:

	std::vector<Sprite> graph_hanger_;

public:



};
