#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// user

struct User
{
    int id;
    string name;
    double balance;
};

// trans

struct Transaction
{
    int txId;
    int sender;
    int receiver;
    double amount;
};

// main cls

class FinTechLedger
{
private:
    // DSA REQUIREMENTS
    // Map -> O(log n) wallet lookup
    map<int, User> users; // Address Book

    // Queue -> FIFO transaction processing
    queue<Transaction> waitingRoom; // Waiting Room

    // Stack -> LIFO undo functionality
    stack<Transaction> undoStack; // Undo Button

    // Hash Map -> O(1) receipt search
    unordered_map<int, Transaction> receipts; // Receipt Finder

    // Graph -> Money flow between users
    unordered_map<int,
                  vector<pair<int, double>>>
        graph; // Money Web

    // EXTRA FEATURES - trans his.

    vector<Transaction> history;

    int nextTxId = 1001;

public:
    // CREATE WALLET

    void createWallet()
    {
        User u;

        cout << "\nEnter User ID: ";
        cin >> u.id;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid ID. Enter numbers only.\n";
            return;
        }

        if (users.find(u.id) != users.end())
        {
            cout << "\nWallet Already Exists!\n";
            return;
        }

        cout << "Enter Name: ";
        cin >> u.name;

        cout << "Enter Balance: ";
        cin >> u.balance;

        users[u.id] = u;

        cout << "\nWallet Created Successfully!\n";
    }

    // TRANSFER MONEY

    void transferMoney()
    {
        Transaction tx;

        cout << "\nSender ID: ";
        cin >> tx.sender;

        cout << "Receiver ID: ";
        cin >> tx.receiver;

        cout << "Amount: ";
        cin >> tx.amount;

        tx.txId = nextTxId++;

        waitingRoom.push(tx);

        cout << "\nTransaction Added To Waiting Room";
        cout << "\nTransaction ID: "
             << tx.txId
             << endl;
    }

    // PROCESS TRANSACTIONS

    void processTransactions()
    {
        if (waitingRoom.empty())
        {
            cout << "\nNo Pending Transactions\n";
            return;
        }

        while (!waitingRoom.empty())
        {
            Transaction tx =
                waitingRoom.front();

            waitingRoom.pop();

            if (users.find(tx.sender) == users.end() ||
                users.find(tx.receiver) == users.end())
            {
                cout
                    << "\nInvalid User IDs\n";

                continue;
            }

            if (users[tx.sender]
                    .balance < tx.amount)
            {
                cout
                    << "\nTX "
                    << tx.txId
                    << " Failed (Low Balance)\n";

                continue;
            }

            users[tx.sender]
                .balance -= tx.amount;

            users[tx.receiver]
                .balance += tx.amount;

            receipts[tx.txId] = tx;

            undoStack.push(tx);

            history.push_back(tx);

            graph[tx.sender]
                .push_back(
                    {tx.receiver,
                     tx.amount});

            cout
                << "\nTX "
                << tx.txId
                << " Successful\n";
        }
    }
    // UNDO LAST TRANSACTION

    void undoLast()
    {
        if (undoStack.empty())
        {
            cout << "\nNothing To Undo\n";
            return;
        }

        Transaction tx =
            undoStack.top();

        undoStack.pop();

        users[tx.sender]
            .balance += tx.amount;

        users[tx.receiver]
            .balance -= tx.amount;

        receipts.erase(tx.txId);

        if (!history.empty())
            history.pop_back();

        if (!graph[tx.sender].empty())
            graph[tx.sender].pop_back();

        cout
            << "\nTransaction "
            << tx.txId
            << " Reversed Successfully\n";
    }

    // DASHBOARD

    void dashboard()
    {
        cout
            << "\n========== DASHBOARD ==========\n";

        double totalMoney = 0;

        for (auto user : users)
        {
            cout
                << "\nID      : "
                << user.second.id;

            cout
                << "\nName    : "
                << user.second.name;

            cout
                << "\nBalance : "
                << user.second.balance;

            cout
                << "\n-------------------------";

            totalMoney +=
                user.second.balance;
        }

        cout
            << "\n\nTotal Wallets : "
            << users.size();

        cout
            << "\nTotal Money   : "
            << totalMoney
            << endl;
    }

    // RECEIPT SEARCH

    void searchReceipt()
    {
        int id;

        cout
            << "\nEnter Transaction ID: ";

        cin >> id;

        if (receipts.find(id) == receipts.end())
        {
            cout
                << "\nReceipt Not Found\n";

            return;
        }

        Transaction tx =
            receipts[id];

        cout
            << "\nTX ID     : "
            << tx.txId;

        cout
            << "\nSender ID : "
            << tx.sender;

        cout
            << "\nReceiver  : "
            << tx.receiver;

        cout
            << "\nAmount    : "
            << tx.amount
            << endl;
    }

    // HISTORY

    void showHistory()
    {
        cout
            << "\n========== HISTORY ==========\n";

        if (history.empty())
        {
            cout
                << "No Transactions\n";

            return;
        }

        for (auto tx : history)
        {
            cout
                << "TX "
                << tx.txId
                << " | "
                << tx.sender
                << " -> "
                << tx.receiver
                << " | "
                << tx.amount
                << endl;
        }
    }
    // DIJKSTRA ALGORITHM
    // Dijkstra -> Cheapest Path
    int runDijkstra()
    {
        vector<vector<pair<int, int>>> g(4);

        g[0].push_back({1, 2});
        g[0].push_back({2, 8});
        g[1].push_back({2, 1});
        g[1].push_back({3, 6});
        g[2].push_back({3, 3});

        vector<int> dist(4, INT_MAX);

        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>>
            pq;

        dist[0] = 0;

        pq.push({0, 0});

        while (!pq.empty())
        {
            int cost = pq.top().first;
            int node = pq.top().second;

            pq.pop();

            for (auto edge : g[node])
            {
                int next = edge.first;
                int wt = edge.second;

                if (dist[next] > cost + wt)
                {
                    dist[next] = cost + wt;

                    pq.push(
                        {dist[next],
                         next});
                }
            }
        }

        return dist[3];
    }

    // GREEDY ALGORITHM
    // Greedy -> Max Approvals
    int runGreedy()
    {
        vector<int> payments =
            {
                50,
                100,
                150,
                200,
                300};

        int limit = 500;

        sort(
            payments.begin(),
            payments.end());

        int approved = 0;

        int sum = 0;

        for (int p : payments)
        {
            if (sum + p <= limit)
            {
                sum += p;
                approved++;
            }
        }

        return approved;
    }

    // ANALYTICS REPORT
    // PQ + Graph + Dijkstra + Greedy

    void analytics()
    {
        cout
            << "\n========== ANALYTICS ==========\n";

        priority_queue<
            pair<double, string>>
            pq;

        for (auto user : users)
        {
            pq.push(
                {user.second.balance,
                 user.second.name});
        }

        cout
            << "\nTOP WALLET HOLDERS\n";

        int rank = 1;

        while (!pq.empty() && rank <= 3)
        {
            cout
                << rank
                << ". "
                << pq.top().second
                << " - "
                << pq.top().first
                << endl;

            pq.pop();

            rank++;
        }

        cout
            << "\nMONEY WEB\n";

        for (auto node : graph)
        {
            for (auto edge : node.second)
            {
                cout
                    << users[node.first].name
                    << " -> "
                    << users[edge.first].name
                    << " ("
                    << edge.second
                    << ")\n";
            }
        }

        cout
            << "\nCheapest Rate (Dijkstra): "
            << runDijkstra();

        cout
            << "\nMax Approvals (Greedy): "
            << runGreedy()
            << endl;
    }
};

int main()
{
    FinTechLedger app;

    int choice;

    do
    {
        cout
            << "\n\n====================================";

        cout
            << "\n         FINTECH LEDGER";

        cout
            << "\n====================================";

        cout
            << "\n1. Create Wallet";
        cout
            << "\n2. Transfer Money";
        cout
            << "\n3. Process Transactions";
        cout
            << "\n4. Undo Last Transaction";
        cout
            << "\n5. Wallet Dashboard";
        cout
            << "\n6. Analytics Report";
        cout
            << "\n7. Search Receipt";
        cout
            << "\n8. Transaction History";
        cout
            << "\n9. Exit";

        cout
            << "\n\nEnter Choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            app.createWallet();
            break;

        case 2:
            app.transferMoney();
            break;

        case 3:
            app.processTransactions();
            break;

        case 4:
            app.undoLast();
            break;

        case 5:
            app.dashboard();
            break;

        case 6:
            app.analytics();
            break;

        case 7:
            app.searchReceipt();
            break;

        case 8:
            app.showHistory();
            break;

        case 9:
            cout
                << "\nThank You!\n";
            break;

        default:
            cout
                << "\nInvalid Choice\n";
        }

    } while (choice != 9);

    return 0;
}