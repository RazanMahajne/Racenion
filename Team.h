//
// Created by Yasmeena Khalaf on 12/07/2026.
//

#ifndef DS_WET2_SPRING_2026_3_TEAM_H
#define DS_WET2_SPRING_2026_3_TEAM_H
#include "wet2util.h"
class ContestantNode;

class Team
{
private:
    int teamId;
    int teamExp;
    int totalMotivation;
    int numContestants;

    Skill collectiveSkill;
    ContestantNode* rootContestant;
    ContestantNode* lastContestant;

public:
    explicit Team(int teamId)
        : teamId(teamId),
          teamExp(0),
          totalMotivation(0),
          numContestants(0),
          collectiveSkill(Skill::identity()),
          rootContestant(nullptr),
          lastContestant(nullptr)
    {
    }

    ~Team() = default;

    Team(const Team&) = delete;
    Team& operator=(const Team&) = delete;

    // Getters
    int getTeamId() const
    {
        return teamId;
    }

    int getTeamExp() const
    {
        return teamExp;
    }

    int getTotalMotivation() const
    {
        return totalMotivation;
    }

    int getNumContestants() const
    {
        return numContestants;
    }

    const Skill& getCollectiveSkill() const
    {
        return collectiveSkill;
    }

    ContestantNode* getRootContestant() const
    {
        return rootContestant;
    }

    ContestantNode* getLastContestant() const
    {
        return lastContestant;
    }

    // Setters
    void setTeamExp(int newTeamExp)
    {
        teamExp = newTeamExp;
    }

    void setTotalMotivation(int newTotalMotivation)
    {
        totalMotivation = newTotalMotivation;
    }

    void setNumContestants(int newNumContestants)
    {
        numContestants = newNumContestants;
    }

    void setCollectiveSkill(const Skill& newCollectiveSkill)
    {
        collectiveSkill = newCollectiveSkill;
    }

    void setRootContestant(ContestantNode* newRootContestant)
    {
        rootContestant = newRootContestant;
    }

    void setLastContestant(ContestantNode* newLastContestant)
    {
        lastContestant = newLastContestant;
    }


    // methods
    void increaseExperience(int amount)
    {
        teamExp += amount;
    }

    void addMotivation(int motivation)
    {
        totalMotivation += motivation;
    }

    void increaseNumContestants()
    {
        numContestants++;
    }

    void multiplyCollectiveSkill(const Skill& skill)
    {
        collectiveSkill *= skill;
    }

    bool isEmpty() const
    {
        return numContestants == 0;
    }
};


#endif //DS_WET2_SPRING_2026_3_TEAM_H
