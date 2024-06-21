#include <string>
#include <rcsc/player/abstract_player_object.h>
#include <rcsc/geom/vector_2d.h>
#include <rcsc/player/world_model.h>
#include <rcsc/common/server_param.h>

template <typename T>
std::string str(T value) { return std::to_string(value); }

std::string PointToDataFrame(rcsc::AbstractPlayerObject::Cont::iterator begin, rcsc::AbstractPlayerObject::Cont::iterator end)
{
    std::string dataRow = "";

    for (rcsc::AbstractPlayerObject::Cont::iterator it = begin; it != end; it++)
    {
        dataRow += str((*it)->pos().x) + ",";
        dataRow += str((*it)->pos().y) + ",";
    }

    return dataRow;
}

std::string PointToDataFrame(rcsc::Vector2D *begin, rcsc::Vector2D *end)
{
    std::string dataRow = "";

    for (rcsc::Vector2D *it = begin; it != end; it++)
        dataRow += str(it->x) + "," + str(it->y) + ",";

    return dataRow;
}

std::string PointToDataFrame(std::vector<rcsc::Vector2D> arr)
{
    std::string dataRow = "";

    for (auto &it : arr)
        dataRow += str(it.x) + "," + str(it.y) + ",";

    return dataRow;
}

std::string LogWorldModel(const rcsc::WorldModel &wm)
{
    typedef std::pair<bool, std::string> player_state;

    std::string state = "";
    const rcsc::ServerParam &SP = rcsc::ServerParam::i();

    state += str(wm.time().cycle()) + "," + str(wm.self().unum()) + "," + str(wm.ball().pos().x) + "," + str(wm.ball().pos().y) + "," + str(wm.ball().vel().x) + "," + str(wm.ball().vel().y) + ",";
    
    std::vector<rcsc::PlayerObject::Cont> players_pointers = {wm.opponentsFromSelf(),
                                                              wm.teammatesFromSelf()};

    for (auto &player_pointer : players_pointers)
    {
        rcsc::Vector2D saw_pos[11];
        rcsc::Vector2D saw_vel[11];
        rcsc::AngleDeg saw_angle[11];

        bool valid_pos[11] = {false, false, false, false, false, false, false, false, false, false, false};
        // add opponents

        for (auto &player : player_pointer)
        {
            if(player == nullptr || player == NULL) continue;
            if(player->posCount() >= 4) continue;

            saw_vel[player->unum()-1] = player->vel(); 
            saw_pos[player->unum()-1] = player->pos();
            saw_angle[player->unum()-1] = player->body();
            valid_pos[player->unum()-1] = true;
        }

        for (int i = 0; i < 11; i++)
        {
            if (valid_pos[i])
            {
                state += str(i+1) + "," + str(saw_pos[i].x) + "," + str(saw_pos[i].y) + "," + str(saw_vel[i].x) + "," + str(saw_vel[i].y) + "," + str(saw_angle[i].radian());
            }
            else
            {
                state += str(i+1) + "," + str(SP.pitchLength()) + "," + str(SP.pitchWidth()) + ",0.0,0.0,0.0";
            }

            if (player_pointer != wm.teammatesFromSelf() || i != 10)
                state += ",";
        }
    }

    return state;
}

std::vector<float> LogWorldModelVector(const rcsc::WorldModel &wm) {
    std::vector<float> state;
    const rcsc::ServerParam &SP = rcsc::ServerParam::i();

    state.push_back(wm.time().cycle());
    state.push_back(wm.self().unum());
    state.push_back(wm.ball().pos().x);
    state.push_back(wm.ball().pos().y);
    state.push_back(wm.ball().vel().x);
    state.push_back(wm.ball().vel().y);

    std::vector<rcsc::PlayerObject::Cont> players_pointers = {wm.opponentsFromSelf(),
                                                              wm.teammatesFromSelf()};

    for (auto &player_pointer : players_pointers)
    {
        rcsc::Vector2D saw_pos[11];
        rcsc::Vector2D saw_vel[11];
        rcsc::AngleDeg saw_angle[11];

        bool valid_pos[11] = {false, false, false, false, false, false, false, false, false, false, false};
        // add opponents

        for (auto &player : player_pointer)
        {
            if(player == nullptr || player == NULL) continue;
            if(player->posCount() >= 4) continue;

            saw_vel[player->unum()-1] = player->vel(); 
            saw_pos[player->unum()-1] = player->pos();
            saw_angle[player->unum()-1] = player->body();
            valid_pos[player->unum()-1] = true;
        }

        for (int i = 0; i < 11; i++)
        {
            if (valid_pos[i])
            {
                state.push_back(i+1);
                state.push_back(saw_pos[i].x);
                state.push_back(saw_pos[i].y);
                state.push_back(saw_vel[i].x);
                state.push_back(saw_vel[i].y);
                state.push_back(saw_angle[i].radian());
            }
            else
            {
                state.push_back(i+1);
                state.push_back(SP.pitchLength());
                state.push_back(SP.pitchWidth());
                state.push_back(0.0);
                state.push_back(0.0);
                state.push_back(0.0);
            }
        }
    }

    return state;
}