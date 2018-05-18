/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_NON_EMPTY_CONTAINER_VALIDATOR_HPP
#define IROHA_NON_EMPTY_CONTAINER_VALIDATOR_HPP

#include "validators/container_validator.hpp"

namespace shared_model {
  namespace validation {

    /**
     * Class that validates blocks and proposal common fieds
     */
    template <typename Iface,
              typename FieldValidator,
              typename TransactionValidator>
    class NonEmptyContainerValidator
        : public ContainerValidator<Iface, FieldValidator> {
     protected:
      void validateTransaction(
          ReasonsGroupType &reason,
          const interface::Transaction &transaction) const {
        auto answer = transaction_validator_.validate(transaction);
        if (answer.hasErrors()) {
          auto message = (boost::format("Tx: %s") % answer.reason()).str();
          reason.second.push_back(message);
        }
      }
      void validateTransactions(
          ReasonsGroupType &reason,
          const interface::types::TransactionsCollectionType &transactions)
          const {
        for (const auto &tx : transactions) {
          validateTransaction(reason, *tx);
        }
      }

     public:
      NonEmptyContainerValidator(
          const TransactionValidator &transaction_validator =
              TransactionValidator(),
          const FieldValidator &field_validator = FieldValidator())
          : transaction_validator_(transaction_validator),
            ContainerValidator<Iface, FieldValidator>::field_validator_(
                field_validator) {}

     private:
      TransactionValidator transaction_validator_;
    };

  }  // namespace validation
}  // namespace shared_model

#endif  // IROHA_NON_EMPTY_CONTAINER_VALIDATOR_HPP
