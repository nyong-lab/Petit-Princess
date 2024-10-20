#include "IGraphics.h"
#include "GraphicsEngine.h"

std::shared_ptr<IGraphics> DXExport::GetInstance()
{
	return std::make_shared<GraphicsEngine>();
}
