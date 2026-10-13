// You can edit anything you want in this file.
// However, you need to implement all public Racenion functions, which are provided below as a template.

#include "Racenion26b2.h"

Racenion::Racenion()
    : teamsById(),
      teamsByMotivation(),
      contestants(),
      unionFind()
{
}

Racenion::~Racenion()
{
    deleteTeams(teamsById.getRoot());
    contestants.forEachValue(
        [](ContestantNode* contestant)
        {
            delete contestant;
        }
    );
}

void Racenion::deleteTeams(TreeNode<int, Team*>* node)
{
    if (node == nullptr)
    {
        return;
    }
    deleteTeams(node->getLeft());
    deleteTeams(node->getRight());

    delete node->getValue();
}

StatusType Racenion::add_team(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    if (teamsById.find(teamId) != nullptr)
    {
        return StatusType::FAILURE;
    }
    Team* newTeam = nullptr;
    bool insertedById = false;

    try
    {
        newTeam = new Team(teamId);

        teamsById.insert(teamId, newTeam);
        insertedById = true;

        teamsByMotivation.insert(MotivationKey(0, teamId), newTeam);

        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        if (insertedById)
        {
            teamsById.remove(teamId);
        }

        delete newTeam;
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Racenion::remove_team(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    try
    {
        TreeNode<int, Team*>* teamNode = teamsById.find(teamId);

        if (teamNode == nullptr)
        {
            return StatusType::FAILURE;
        }

        Team* team = teamNode->getValue();

        MotivationKey motivationKey(team->getTotalMotivation(), teamId);

        if (!team->isEmpty())
        {
            team->getRootContestant()->setTeam(nullptr);
        }


        teamsByMotivation.remove(motivationKey);
        teamsById.remove(teamId);

        delete team;

        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Racenion::add_contestant(int contestantId, int teamId, const Skill& skill, int motivation, int missionsHad)
{
    if (contestantId <= 0 || teamId <= 0 || !skill.isValid() || motivation < 0 || missionsHad < 0)
    {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<int, Team*>* teamNode = teamsById.find(teamId);

    if (teamNode == nullptr ||
        contestants.find(contestantId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    Team* team = teamNode->getValue();
    ContestantNode* newContestant = nullptr;

    MotivationKey oldMotivationKey(team->getTotalMotivation(), teamId);

    MotivationKey newMotivationKey(team->getTotalMotivation() + motivation, teamId);

    try
    {
        newContestant = new ContestantNode(contestantId, motivation, missionsHad, skill);
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }


    if (motivation != 0)
    {
        try
        {
            teamsByMotivation.insert(newMotivationKey, team);
        }
        catch (const std::bad_alloc&)
        {
            delete newContestant;
            return StatusType::ALLOCATION_ERROR;
        }
    }

    try
    {
        bool inserted = contestants.insert(contestantId, newContestant);

        if (!inserted)
        {
            if (motivation != 0)
            {
                teamsByMotivation.remove(newMotivationKey);
            }
            delete newContestant;
            return StatusType::FAILURE;
        }
    }
    catch (const std::bad_alloc&)
    {
        if (motivation != 0)
        {
            teamsByMotivation.remove(newMotivationKey);
        }
        delete newContestant;
        return StatusType::ALLOCATION_ERROR;
    }

    if (motivation != 0)
    {
        teamsByMotivation.remove(oldMotivationKey);
    }

    Skill newCollectiveSkill = team->getCollectiveSkill() * skill;

    if (team->isEmpty())
    {
        unionFind.makeSet(newContestant, team);

        team->setRootContestant(newContestant);
    }

    else
    {
        ContestantNode* root = unionFind.addContestantToTeam(team->getRootContestant(),
                                                             newContestant, newCollectiveSkill, team);
        team->setRootContestant(root);
    }

    team->setLastContestant(newContestant);

    team->addMotivation(motivation);

    team->increaseNumContestants();

    team->setCollectiveSkill(newCollectiveSkill);

    return StatusType::SUCCESS;
}

output_t<int> Racenion::duel(int teamId1, int teamId2)
{
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
    {
        return StatusType::INVALID_INPUT;
    }

    try
    {
        TreeNode<int, Team*>* teamNode1 = teamsById.find(teamId1);
        TreeNode<int, Team*>* teamNode2 = teamsById.find(teamId2);

        if (teamNode1 == nullptr || teamNode2 == nullptr)
        {
            return StatusType::FAILURE;
        }

        Team* team1 = teamNode1->getValue();
        Team* team2 = teamNode2->getValue();

        if (team1->isEmpty() || team2->isEmpty())
        {
            return StatusType::FAILURE;
        }

        int effectiveScore1 = team1->getTeamExp() + team1->getTotalMotivation();
        int effectiveScore2 = team2->getTeamExp() + team2->getTotalMotivation();
        int result;

        if (effectiveScore1 > effectiveScore2)
        {
            team1->increaseExperience(3);
            result = 1;
        }
        else if (effectiveScore2 > effectiveScore1)
        {
            team2->increaseExperience(3);
            result = 3;
        }
        else
        {
            const Skill& skill1 = team1->getCollectiveSkill();

            const Skill& skill2 = team2->getCollectiveSkill();

            if (skill1 > skill2)
            {
                team1->increaseExperience(3);
                result = 2;
            }
            else if (skill2 > skill1)
            {
                team2->increaseExperience(3);
                result = 4;
            }
            else
            {
                team1->increaseExperience(1);
                team2->increaseExperience(1);
                result = 0;
            }
        }
        team1->getRootContestant()->increaseRootMissions(1);
        team2->getRootContestant()->increaseRootMissions(1);

        return result;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Racenion::get_contestant_missions_number(int contestantId)
{
    if (contestantId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    try
    {
        ContestantNode** contestantPtr = contestants.find(contestantId);

        if (contestantPtr == nullptr)
        {
            return StatusType::FAILURE;
        }

        ContestantNode* contestant = *contestantPtr;

        int totalMissions = 0;
        Skill ignoredSkill = Skill::identity();

        unionFind.find(contestant, totalMissions, ignoredSkill);

        return totalMissions;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Racenion::get_team_experience(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    try
    {
        TreeNode<int, Team*>* teamNode = teamsById.find(teamId);

        if (teamNode == nullptr)
        {
            return StatusType::FAILURE;
        }

        Team* team = teamNode->getValue();
        return team->getTeamExp();
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Racenion::get_ith_collective_motivation_team(int i)
{
    try
    {
        if (i < 1 || i > teamsByMotivation.getSize())
        {
            return StatusType::FAILURE;
        }

        TreeNode<MotivationKey, Team*>* node = teamsByMotivation.findIth(i);

        if (node == nullptr)
        {
            return StatusType::FAILURE;
        }

        Team* team = node->getValue();

        return team->getTeamId();
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<Skill> Racenion::get_partial_team_skill(int contestantId)
{
    if (contestantId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    try
    {
        ContestantNode** contestantPtr = contestants.find(contestantId);

        if (contestantPtr == nullptr)
        {
            return StatusType::FAILURE;
        }

        ContestantNode* contestant = *contestantPtr;

        int ignoredMissions = 0;
        Skill partialSkill = Skill::identity();

        ContestantNode* root = unionFind.find(contestant, ignoredMissions, partialSkill);

        if (root == nullptr || root->getTeam() == nullptr)
        {
            return StatusType::FAILURE;
        }

        return partialSkill;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Racenion::recruit(int recruitingTeamId, int recruitedTeamId)
{
    if (recruitingTeamId <= 0 || recruitedTeamId <= 0 || recruitingTeamId == recruitedTeamId)
    {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<int, Team*>* recruitingNode = teamsById.find(recruitingTeamId);
    TreeNode<int, Team*>* recruitedNode = teamsById.find(recruitedTeamId);

    if (recruitingNode == nullptr || recruitedNode == nullptr)
    {
        return StatusType::FAILURE;
    }

    Team* recruitingTeam = recruitingNode->getValue();
    Team* recruitedTeam = recruitedNode->getValue();

    if (recruitingTeam->isEmpty())
    {
        return StatusType::FAILURE;
    }

    if (!recruitedTeam->isEmpty())
    {
        int recruitingPower =
            recruitingTeam->getTeamExp() +
            recruitingTeam->getTotalMotivation() +
            recruitingTeam->getCollectiveSkill().getEffectiveSkill();

        int recruitedPower =
            recruitedTeam->getTeamExp() +
            recruitedTeam->getTotalMotivation() +
            recruitedTeam->getCollectiveSkill().getEffectiveSkill();

        if (recruitingPower <= recruitedPower)
        {
            return StatusType::FAILURE;
        }
    }

    int oldRecruitingMotivation = recruitingTeam->getTotalMotivation();
    int recruitedMotivation = recruitedTeam->getTotalMotivation();
    int newTotalMotivation = oldRecruitingMotivation + recruitedMotivation;

    MotivationKey oldRecruitingKey(oldRecruitingMotivation, recruitingTeamId);

    MotivationKey recruitedKey(recruitedMotivation, recruitedTeamId);

    MotivationKey newRecruitingKey(newTotalMotivation, recruitingTeamId);

    bool motivationKeyChanges = newTotalMotivation != oldRecruitingMotivation;

    bool insertedNewMotivationKey = false;

    try
    {
        if (motivationKeyChanges)
        {
            teamsByMotivation.insert(newRecruitingKey, recruitingTeam);

            insertedNewMotivationKey = true;
        }

        Skill oldRecruitingSkill = recruitingTeam->getCollectiveSkill();

        Skill recruitedSkill = recruitedTeam->getCollectiveSkill();

        Skill newCollectiveSkill =
            oldRecruitingSkill * recruitedSkill;

        int newExperience = recruitingTeam->getTeamExp() + recruitedTeam->getTeamExp();

        int newNumContestants = recruitingTeam->getNumContestants() +
            recruitedTeam->getNumContestants();

        ContestantNode* recruitingRoot = recruitingTeam->getRootContestant();

        ContestantNode* recruitedRoot = recruitedTeam->getRootContestant();

        ContestantNode* newRoot =
            unionFind.unionTeams(recruitingRoot, recruitedRoot, oldRecruitingSkill, recruitingTeam);

        if (motivationKeyChanges)
        {
            teamsByMotivation.remove(oldRecruitingKey);
        }

        teamsByMotivation.remove(recruitedKey);
        teamsById.remove(recruitedTeamId);

        recruitingTeam->setTeamExp(newExperience);
        recruitingTeam->setTotalMotivation(newTotalMotivation);
        recruitingTeam->setNumContestants(newNumContestants);
        recruitingTeam->setCollectiveSkill(newCollectiveSkill);
        recruitingTeam->setRootContestant(newRoot);

        if (!recruitedTeam->isEmpty())
        {
            recruitingTeam->setLastContestant(recruitedTeam->getLastContestant());
        }

        delete recruitedTeam;

        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        if (insertedNewMotivationKey)

            teamsByMotivation.remove(newRecruitingKey);


        return StatusType::ALLOCATION_ERROR;
    }
}
