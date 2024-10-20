#include "GameDemo_pch.h"
#include "MerchantScript.h"

void MerchantScript::Update(double dt)
{

}

std::shared_ptr<Component> MerchantScript::Clone() const
{
    return std::shared_ptr<MerchantScript>();
}
