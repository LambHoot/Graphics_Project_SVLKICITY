#pragma once
class Building
{

private:
	float height;
	float width;
	glm::vec3 center;
	vector<glm::vec3> bldgPoints;


public:
	Building(float h, float w, glm::vec3 c);
	~Building();

	vector<glm::vec3> generateBldg(float h, float w, glm::vec3 c) { return bldgPoints; }
	vector<glm::vec3> generateRandomBldg(float wRangeLO, float wRangeHI, float hRangeLO, float hRangeHI, glm::vec3 c) { return bldgPoints; }
	vector<glm::vec3> getBldgPoints() { return bldgPoints; }
};

