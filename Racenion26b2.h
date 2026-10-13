//
// 234218 Data Structures 1.
// Semester: 2026 B (Spring).
// Wet Exercise #2 - Racenion.
//
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
//

#ifndef RACENION26B2_H_
#define RACENION26B2_H_
#include "AVLTree.h"
#include "wet2util.h"
#include "ContestantNode.h"
#include "HashTable.h"
#include "Team.h"
#include "UnionFind.h"


class MotivationKey {
private:
	int totalMotivation;
	int teamId;

public:
	MotivationKey(int motivation = 0, int id = 0)
		: totalMotivation(motivation), teamId(id) {}

	int getTotalMotivation() const {
		return totalMotivation;
	}

	int getTeamId() const {
		return teamId;
	}

	bool operator<(const MotivationKey& other) const {
		if (totalMotivation != other.totalMotivation) {
			return totalMotivation < other.totalMotivation;
		}

		return teamId < other.teamId;
	}

	bool operator>(const MotivationKey& other) const {
		return other < *this;
	}

	bool operator==(const MotivationKey& other) const {
		return totalMotivation == other.totalMotivation &&
			   teamId == other.teamId;
	}
};

class Racenion {
private:

	AVLTree<int, Team*> teamsById;
	AVLTree<MotivationKey, Team*> teamsByMotivation;
	HashTable<int, ContestantNode*> contestants;
	UnionFind unionFind;

	static void deleteTeams(TreeNode<int, Team*>* node);

public:
	// <DO-NOT-MODIFY> {

	Racenion();
	virtual ~Racenion();

	StatusType add_team(int teamId);

	StatusType remove_team(int teamId);

	StatusType add_contestant(int contestantId,
	                          int teamId,
	                          const Skill &skill,
	                          int motivation,
	                          int missionsHad);

	output_t<int> duel(int teamId1, int teamId2);

	output_t<int> get_contestant_missions_number(int contestantId);

	output_t<int> get_team_experience(int teamId);

	output_t<int> get_ith_collective_motivation_team(int i);

	output_t<Skill> get_partial_team_skill(int contestantId);

	StatusType recruit(int recruitingTeamId, int recruitedTeamId);

	// } </DO-NOT-MODIFY>
};

#endif // RACENION26B2_H_
