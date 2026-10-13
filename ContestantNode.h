//
// Created by Yasmeena Khalaf on 12/07/2026.
//

#ifndef DS_WET2_SPRING_2026_3_CONTESTANTNODE_H
#define DS_WET2_SPRING_2026_3_CONTESTANTNODE_H


#include "wet2util.h"

class Team;

class ContestantNode {
private:
    int contestantId;
    int motivation;
    int missionsHad;

    Skill personalSkill;

    // Union-Find fields
    ContestantNode* parent;
    int setSize;
    int missionsToParent;
    int rootMissions;
    Skill skillToParent;
    Skill rootPrefixSkill;
    Team* team;

public:
    ContestantNode(int contestantId,
                   int motivation,
                   int missionsHad,
                   const Skill& skill)
        : contestantId(contestantId),
          motivation(motivation),
          missionsHad(missionsHad),
          personalSkill(skill),
          parent(nullptr),
          setSize(1),
          missionsToParent(0),
          rootMissions(0),
          skillToParent(Skill::identity()),
          rootPrefixSkill(skill),
          team(nullptr)
    {
    }

    ~ContestantNode() = default;

    ContestantNode(const ContestantNode&) = delete;
    ContestantNode& operator=(const ContestantNode&) = delete;

    //getters

    int getContestantId() const {
        return contestantId;
    }

    int getMotivation() const {
        return motivation;
    }

    int getInitialMissions() const {
        return missionsHad;
    }

    const Skill& getPersonalSkill() const {
        return personalSkill;
    }

    ContestantNode* getParent() const {
        return parent;
    }

    int getSetSize() const {
        return setSize;
    }

    int getMissionsToParent() const {
        return missionsToParent;
    }

    int getRootMissions() const {
        return rootMissions;
    }

    const Skill& getSkillToParent() const {
        return skillToParent;
    }

    const Skill& getRootPrefixSkill() const {
        return rootPrefixSkill;
    }

    Team* getTeam() const {
        return team;
    }

    bool isRoot() const {
        return parent == nullptr;
    }

    //setters

    void setParent(ContestantNode* newParent) {
        parent = newParent;
    }

    void setSetSize(int newSize) {
        setSize = newSize;
    }

    void setMissionsToParent(int missions) {
        missionsToParent = missions;
    }

    void setRootMissions(int missions) {
        rootMissions = missions;
    }

    void setSkillToParent(const Skill& skill) {
        skillToParent = skill;
    }

    void setRootPrefixSkill(const Skill& skill) {
        rootPrefixSkill = skill;
    }

    void setTeam(Team* newTeam) {
        team = newTeam;
    }

    //methods
    void increaseSetSize(int amount) {
        setSize += amount;
    }

    void increaseRootMissions(int amount) {
        rootMissions += amount;
    }
};

#endif //DS_WET2_SPRING_2026_3_CONTESTANTNODE_H
