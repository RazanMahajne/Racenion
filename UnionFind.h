//
// Created by Yasmeena Khalaf on 12/07/2026.
//

#ifndef DS_WET2_SPRING_2026_3_UNIONFIND_H
#define DS_WET2_SPRING_2026_3_UNIONFIND_H


#include "ContestantNode.h"
#include "Team.h"

class UnionFind
{
    ContestantNode* findRecursive(ContestantNode* node, int& totalMissions, Skill& partialSkill)
    {
        if (node->isRoot())
        {
            totalMissions = node->getRootMissions();
            partialSkill = node->getRootPrefixSkill();
            return node;
        }

        ContestantNode* oldParent = node->getParent();
        int oldMissionDifference = node->getMissionsToParent();
        Skill oldSkillDifference = node->getSkillToParent();

        int parentMissions = 0;
        Skill parentPartialSkill = Skill::identity();

        ContestantNode* root = findRecursive(oldParent, parentMissions, parentPartialSkill);

        totalMissions = parentMissions + oldMissionDifference;
        partialSkill = parentPartialSkill * oldSkillDifference;


        node->setParent(root);
        node->setMissionsToParent(totalMissions - root->getRootMissions());
        node->setSkillToParent(root->getRootPrefixSkill().inv() * partialSkill);

        return root;
    }

public:
    UnionFind() = default;
    ~UnionFind() = default;

    UnionFind(const UnionFind&) = delete;
    UnionFind& operator=(const UnionFind&) = delete;


    void makeSet(ContestantNode* contestant, Team* team)
    {
        if (contestant == nullptr)
        {
            return;
        }

        contestant->setParent(nullptr);
        contestant->setSetSize(1);

        contestant->setMissionsToParent(0);
        contestant->setRootMissions(contestant->getInitialMissions());

        contestant->setSkillToParent(Skill::identity());
        contestant->setRootPrefixSkill(contestant->getPersonalSkill());

        contestant->setTeam(team);
    }

    ContestantNode* find(ContestantNode* contestant, int& totalMissions, Skill& partialSkill)
    {
        if (contestant == nullptr)
        {
            totalMissions = 0;
            partialSkill = Skill::identity();
            return nullptr;
        }

        return findRecursive(contestant, totalMissions, partialSkill);
    }

    ContestantNode* findRoot(ContestantNode* contestant)
    {
        int ignoredMissions = 0;
        Skill ignoredSkill = Skill::identity();
        return find(contestant, ignoredMissions, ignoredSkill);
    }

    ContestantNode* addContestantToTeam(ContestantNode* teamRoot, ContestantNode* newContestant,
                                        const Skill& newPartialSkill, Team* team)
    {
        if (teamRoot == nullptr || newContestant == nullptr || team == nullptr)
        {
            return teamRoot;
        }

        if (!teamRoot->isRoot())
        {
            return teamRoot;
        }

        if (!newContestant->isRoot() || newContestant->getSetSize() != 1)
        {
            return teamRoot;
        }

        if (teamRoot == newContestant)
        {
            return teamRoot;
        }

        newContestant->setParent(teamRoot);

        newContestant->setMissionsToParent(newContestant->getInitialMissions() - teamRoot->getRootMissions());

        newContestant->setSkillToParent(teamRoot->getRootPrefixSkill().inv() * newPartialSkill);

        newContestant->setTeam(nullptr);
        teamRoot->setSetSize(teamRoot->getSetSize() + 1);
        teamRoot->setTeam(team);

        return teamRoot;
    }


    ContestantNode* unionTeams(ContestantNode* recruitingRoot, ContestantNode* recruitedRoot,
                               const Skill& recruitingCollectiveSkill, Team* recruitingTeam)
    {
        if (recruitingRoot == nullptr)
        {
            return nullptr;
        }

        if (recruitedRoot == nullptr)
        {
            recruitingRoot->setTeam(recruitingTeam);
            return recruitingRoot;
        }

        recruitingRoot = findRoot(recruitingRoot);
        recruitedRoot = findRoot(recruitedRoot);

        if (recruitingRoot == recruitedRoot)
        {
            recruitingRoot->setTeam(recruitingTeam);
            return recruitingRoot;
        }

        int recruitingSize = recruitingRoot->getSetSize();
        int recruitedSize = recruitedRoot->getSetSize();

        int recruitingRootMissions = recruitingRoot->getRootMissions();
        int recruitedRootMissions = recruitedRoot->getRootMissions();

        Skill recruitingRootPrefix = recruitingRoot->getRootPrefixSkill();
        Skill recruitedRootPrefix = recruitedRoot->getRootPrefixSkill();

        Skill shiftedRecruitedRootPrefix = recruitingCollectiveSkill * recruitedRootPrefix;

        if (recruitingSize >= recruitedSize)
        {
            recruitedRoot->setParent(recruitingRoot);

            recruitedRoot->setMissionsToParent(recruitedRootMissions - recruitingRootMissions);

            recruitedRoot->setSkillToParent(recruitingRootPrefix.inv() * shiftedRecruitedRootPrefix);

            recruitingRoot->setSetSize(recruitingSize + recruitedSize);

            recruitedRoot->setTeam(nullptr);
            recruitingRoot->setTeam(recruitingTeam);

            return recruitingRoot;
        }

        recruitedRoot->setRootPrefixSkill(shiftedRecruitedRootPrefix);

        recruitingRoot->setParent(recruitedRoot);

        recruitingRoot->setMissionsToParent(recruitingRootMissions - recruitedRootMissions);

        recruitingRoot->setSkillToParent(shiftedRecruitedRootPrefix.inv() * recruitingRootPrefix);

        recruitedRoot->setSetSize(recruitingSize + recruitedSize);

        recruitingRoot->setTeam(nullptr);
        recruitedRoot->setTeam(recruitingTeam);

        return recruitedRoot;
    }
};

#endif //DS_WET2_SPRING_2026_3_UNIONFIND_H
