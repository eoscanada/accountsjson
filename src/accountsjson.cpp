#include "accountsjson.hpp"

EOSIO_ABI(accountsjson, (set))

#define VALIDATE_JSON(Variable, MAX_SIZE)\
::accountsjson::validate_json(\
    Variable,\
    MAX_SIZE,\
    #Variable " must be a JSON object (if specified).",\
    #Variable " should be shorter than " #MAX_SIZE " bytes."\
)

// @abi
void accountsjson::set(
    const account_name owner,
    const string& json
) {
    require_auth(owner);

    VALIDATE_JSON(json, 8192);

    accounts accounts_table(_self, _self);

    update_accounts(accounts_table, owner, [&](auto& row) {
        row.json = json;
    });
}

/// Helpers

void accountsjson::update_accounts(
    accounts& accounts_table,
    const account_name owner,
    const function<void(accounts_row&)> updater
) {
    auto itr = accounts_table.find(owner);
    if (itr == accounts_table.end()) {
        accounts_table.emplace(owner, [&](auto& row) {
            row.owner = owner;
            row.created_at = time_point_sec(now());
            row.updated_at = time_point_sec(now());
            updater(row);
        });
    } else {
        accounts_table.modify(itr, owner, [&](auto& row) {
            row.updated_at = time_point_sec(now());
            updater(row);
        });
    }
}


// Do not use directly, use the VALIDATE_JSON macro instead!
void accountsjson::validate_json(
    const string& payload,
    size_t max_size,
    const char* not_object_message,
    const char* over_size_message
) {
    if (payload.size() <= 0) return;

    eosio_assert(payload[0] == '{', not_object_message);
    eosio_assert(payload.size() < max_size, over_size_message);
}

