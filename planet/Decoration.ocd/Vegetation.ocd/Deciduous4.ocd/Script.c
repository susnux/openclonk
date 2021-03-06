/*-- Yet Another Additional Deciduous Tree --*/

#include Library_Plant
#include Library_Tree

private func SeedChance() { return 500; }
private func SeedArea() { return 400; }
private func SeedAmount() { return 10; }

local lib_tree_burned = Deco_Tree_Deciduous4_Burned;

public func GetTreetopPosition(pos)
{
	return Shape->Rectangle(-30,-30, 60,80)->GetRandomPoint(pos);
}

local Name = "$Name$";
local Touchable = 0;
local BlastIncinerate = 2;
local ContactIncinerate = 6;
local NoBurnDecay = 1;
local Components = {Wood = 4};