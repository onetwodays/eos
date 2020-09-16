#pragma once

#include <string>        // for string printing
#include <system_error>  // bring in std::error_code et al

// This is the custom error code enum
enum class block_vault_errors
{
  success        = 0, // 0 should not represent an error
  conflict_block = 1,
  outdated_snapshot = 2
};

namespace std
{
  // Tell the C++ 11 STL metaprogramming that enum ConversionErrc
  // is registered with the standard error code system
  template <> struct is_error_code_enum<block_vault_errors> : true_type
  {
  };
}

namespace detail
{
  // Define a custom error code category derived from std::error_category
  class block_vault_errors_category : public std::error_category
  {
  public:
    // Return a short descriptive name for the category
    virtual const char *name() const noexcept override final { return "BlockVaultErrors"; }
    // Return what each enum means in text
    virtual std::string message(int c) const override final
    {
      switch (static_cast<block_vault_errors>(c))
      {
      case block_vault_errors::success:
        return "success";
      case block_vault_errors::conflict_block:
        return "The proposed constructed block is in conflict with the state of BlockVault and is rejected";
      case block_vault_errors::outdated_snapshot:
        return "The proposed snapshot is outdated";
      default:
        return "unknown";
      }
    }
  };
}