/**
	Chine
	A chine with a large waterfall and lots of vegetation.
	
	@author Maikel
*/


// Scenario properties which can be set later by the lobby options.
static const SCENOPT_Material = 1; // Amount of material available from start.
static const SCENOPT_MapSize = 1; // Size of the map.
static const SCENOPT_Difficulty = 1; // Difficulty settings.

protected func Initialize()
{
	// Rules: team account and buying at flagpole.
	CreateObject(Rule_TeamAccount);
	CreateObject(Rule_BuyAtFlagpole);
	
	// Goal: transport the cannon to the top of the chine.
	var cannon = CreateObject(Cannon, 96 + RandomX(-12, 12), LandscapeHeight() - 92);
	cannon->CreateContents(PowderKeg);
	var cannon_goal = CreateObject(Goal_Script);
	cannon_goal.Name = "$GoalName$";
	cannon_goal.Description = "$GoalDesc$";
	// Add an effect to check whether the goal is fulfilled.
	AddEffect("GoalCheck", nil, 100, 2, nil);
	
	// Initialize different parts of the scenario.
	InitEnvironment(SCENOPT_Difficulty);
	InitVegetation(SCENOPT_MapSize);
	InitAnimals();
	InitMaterial(SCENOPT_Material);	
	return;
}


/*-- Player Initialization --*/

protected func InitializePlayer(int plr)
{ 
	// Harsh zoom range.
	SetPlayerZoomByViewRange(plr, 400, nil, PLRZOOM_Direct | PLRZOOM_LimitMax);
	SetPlayerViewLock(plr, true);

	// Move clonks to location and give them a shovel.
	var index = 0, crew;
	while (crew = GetCrew(plr, index))
	{
		crew->SetPosition(96 + RandomX(-12, 12), LandscapeHeight() - 92);

		// First clonk can construct, others can chop.
		if (index == 0)
		{
			crew->CreateContents(Shovel);
			crew->CreateContents(Pickaxe);
		}
		else
		{
			crew->CreateContents(Axe);
			crew->CreateContents(Hammer);
		}
		index++;
	}
	
	// Give the player basic knowledge.
	GivePlayerBasicKnowledge(plr);
	GivePlayerSpecificKnowledge(plr, [InventorsLab, Ropeladder, MetalBarrel, PowderKeg, GrappleBow]);
	
	// Give the player the elementary base materials and some tools.
	GivePlayerElementaryBaseMaterial(plr);
	GivePlayerToolsBaseMaterial(plr);
	return;
}


/*-- Goal Check --*/

global func FxGoalCheckTimer(object target, proplist effect)
{
	var cannon = FindObject(Find_ID(Cannon));
	if (!cannon)
	{
		// Start elimination dialogue due to lost cannon.
		// TODO
		return -1;
	}
	if (cannon->GetY() < 100)
	{
		var goal = FindObject(Find_ID(Goal_Script));
		if (goal)
			goal->Fulfill();
		return -1;	
	}
	return 1;
}


/*-- Scenario Initialization --*/

private func InitEnvironment(int difficulty)
{
	// Adjust the sky a bit.
	SetSkyParallax(0, 20, 20);
	SetSkyAdjust(RGBa(225, 255, 205, 191), RGB(63, 200, 0));
	
	// Waterfalls dominate the landscape.
	for (var i = 0; i < 20; i++)
	{
		var x = Random(LandscapeWidth());
		if (!GBackSky(x, 0))
			continue;
		var fall = CreateWaterfall(x, 0, RandomX(3, 5), "Water");
		fall->SetDirection(0, 4, 4, 4);
		fall->SetSoundLocation(LandscapeWidth() / 2, Random(LandscapeHeight()));
	}
	// Cast some additional PXS at the start at random locations.
	for (var i = 0; i < 25000; i++)
		InsertMaterial(Material("Water"), Random(LandscapeWidth()), Random(5 * LandscapeHeight() / 6), RandomX(-5, 5), RandomX(3, 6));
	
	// Some natural disasters. 
	Earthquake->SetChance(2 + 2 * difficulty);
	return;
}

private func InitVegetation(int map_size)
{
	// Define parts of the map for even distribution.
	var top = Rectangle(0, 0, LandscapeWidth(), LandscapeHeight() / 3);
	var middle = Rectangle(0, LandscapeHeight() / 3, LandscapeWidth(), LandscapeHeight() / 3);
	var bottom = Rectangle(0, 2 * LandscapeHeight() / 3, LandscapeWidth(), LandscapeHeight() / 3);
	
	// Place gras wherever possible.
	PlaceGrass(100);
	
	// Place some cocont trees and cave mushrooms for wood.
	for (var i = 0; i < 16 + Random(4); i++)
	{
		PlaceVegetation(Tree_Coconut, top.x, top.y, top.w, top.h, 1000 * (61 + Random(40)));
		PlaceVegetation(Tree_Coconut, middle.x, middle.y, middle.w, middle.h, 1000 * (61 + Random(40)));
		PlaceVegetation(Tree_Coconut, bottom.x, bottom.y, bottom.w, bottom.h, 1000 * (61 + Random(40)));
	}
	LargeCaveMushroom->Place(4, middle, { terraform = false });
	LargeCaveMushroom->Place(4, bottom, { terraform = false });
		
	// Place some bushes, ferns and mushrooms.
	SproutBerryBush->Place(2, top);
	SproutBerryBush->Place(2, middle);
	SproutBerryBush->Place(2, bottom);
	Fern->Place(20, top);
	Fern->Place(20, middle);
	Fern->Place(20, bottom);
	Mushroom->Place(14, top);
	Mushroom->Place(14, middle);
	Mushroom->Place(14, bottom);
	
	// Some ranks and trunks.
	for (var i = 0; i < 4 + Random(3); i++)
	{
		PlaceVegetation(Rank, top.x, top.y, top.w, top.h, 1000 * (61 + Random(40)));
		PlaceVegetation(Rank, middle.x, middle.y, middle.w, middle.h, 1000 * (61 + Random(40)));
		PlaceVegetation(Rank, bottom.x, bottom.y, bottom.w, bottom.h, 1000 * (61 + Random(40)));
		PlaceVegetation(Trunk, top.x, top.y, top.w, top.h, 1000 * (61 + Random(40)));
		PlaceVegetation(Trunk, middle.x, middle.y, middle.w, middle.h, 1000 * (61 + Random(40)));
		PlaceVegetation(Trunk, bottom.x, bottom.y, bottom.w, bottom.h, 1000 * (61 + Random(40)));
	}
	
	// Some objects in the earth.	
	PlaceObjects(Rock, 25 + 10 * map_size + Random(10),"Earth");
	PlaceObjects(Firestone, 20 + 10 * map_size + Random(5), "Earth");
	PlaceObjects(Loam, 20 + 10 * map_size + Random(5), "Earth");
	return;
}

private func InitAnimals()
{
	return;
}

private func InitMaterial(int amount)
{
	// No extra materials for little materials.
	if (amount <= 1)
		return;
		
	// For medium amount of materials provide a lorry with resources.	
	if (amount >= 2)
	{
		var lorry = CreateObject(Lorry, 96 + RandomX(-12, 12), LandscapeHeight() - 92);
		lorry->CreateContents(Wood, 6);
		lorry->CreateContents(Metal, 4);
		lorry->CreateContents(Rock, 4);
		lorry->CreateContents(Dynamite, 4);
		lorry->CreateContents(Loam, 4);
		
		// For large amount of materials provide some buildings as well.
		if (amount >= 3)
		{
			lorry->CreateContents(Wood, 6);
			lorry->CreateContents(Metal, 4);
			lorry->CreateContents(Rock, 4);
			lorry->CreateContents(DynamiteBox, 4);
			lorry->CreateContents(Ropeladder, 4);	
		}		
	}
	return;
}


/*-- Some helper functions --*/

global func TestGoldCount()
{
	var pos;
	while (pos = FindLocation(Loc_Material("Gold")))
	{
		var pos = CreateObject(Rock, pos.x, pos.y)->Explode(100);
	}
	var gold_count = ObjectCount(Find_ID(Nugget));
	return gold_count;
}
