#pragma once

class MatchController
{
public:
    MatchController(const std::string& matchApiEndpoint)
    : mMatchApiEndpoint(matchApiEndpoint), mMatchStarted(false)
    {}

    

    void RequestMatch();

    bool CheckMatchStatus();

private:

    const std::string mMatchApiEndpoint;
    
    std::atomic_bool mMatchStarted;
    std::string mMatchTicketId;

};

extern std::unique_ptr<MatchController> GMatchMaker;