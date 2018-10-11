#pragma once

#include <algorithm>
#include <string>

#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>

using eosio::name;
using eosio::time_point_sec;
using std::function;
using std::string;

class accountsjson : public eosio::contract {
    public:
        accountsjson(account_name self)
        :eosio::contract(self)
        {}
        // @abi
        void set(
            const account_name owner,
            const string& json
        );


    private:
        struct accounts_row {
            account_name          owner;
            string                json;

            time_point_sec        created_at;
            time_point_sec        updated_at;

            auto primary_key()const { return owner; }
        };
        typedef eosio::multi_index<
            N(accountsjson), accounts_row
        > accounts;

       void update_accounts(
            accounts& accounts_table,
            const account_name owner,
            const function<void(accounts_row&)> updater
        );

        // Do not use directly, use the VALIDATE_JSON macro instead!
        void validate_json(
            const string& payload,
            size_t max_size,
            const char* not_object_message,
            const char* over_size_message
        );
};
