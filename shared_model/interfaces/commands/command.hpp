/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
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

#ifndef IROHA_SHARED_MODEL_COMMAND_HPP
#define IROHA_SHARED_MODEL_COMMAND_HPP

#include <boost/variant.hpp>
#include <utility>
#include "interfaces/commands/add_asset_quantity.hpp"
#include "interfaces/commands/add_peer.hpp"
#include "interfaces/commands/add_signatory.hpp"
#include "interfaces/commands/append_role.hpp"
#include "interfaces/commands/create_account.hpp"
#include "interfaces/commands/create_asset.hpp"
#include "interfaces/commands/create_domain.hpp"
#include "interfaces/commands/create_role.hpp"
#include "interfaces/commands/grant_permission.hpp"
#include "interfaces/commands/remove_signatory.hpp"
#include "interfaces/commands/revoke_permission.hpp"
#include "interfaces/commands/set_quorum.hpp"
#include "interfaces/commands/transfer_asset.hpp"
#include "interfaces/polymorphic_wrapper.hpp"
#include "interfaces/hashable.hpp"
#include "interfaces/visitor_apply_for_all.hpp"
#include "model/command.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Class provides commands container for all commands in system.
     * General note: this class is container for commands, not a base class.
     */
    class Command : public Hashable<Command, iroha::model::Command> {
     private:
      /// Shortcut type for polymorphic wrapper
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;

     public:
      /// Type of variant, that handle concrete command
      using CommandVariantType = boost::variant<w<AddAssetQuantity>>;
//                                                w<AddPeer>,
//                                                w<AddSignatory>,
//                                                w<AppendRole>,
//                                                w<CreateAccount>,
//                                                w<CreateAsset>,
//                                                w<CreateDomain>,
//                                                w<CreateRole>,
//                                                w<GrantPermission>,
//                                                w<RemoveSignatory>,
//                                                w<RevokePermission>,
//                                                w<SetQuorum>,
//                                                w<TransferAsset>>;

      /// Types of concrete commands, in attached variant
      using CommandListType = CommandVariantType::types;

      /**
       * @return reference to const variant with concrete command
       */
      virtual const CommandVariantType &get() const = 0;

      // ------------------------| Primitive override |-------------------------

      std::string toString() const override {
        return boost::apply_visitor(detail::ToStringVisitor(), get());
      }

      OldModelType *makeOldModel() const override {
        return boost::apply_visitor(
            detail::OldModelCreatorVisitor<OldModelType *>(), get());
      }

      bool operator==(const ModelType &rhs) const override {
        return this->get() == rhs.get();
      }
    };

  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_COMMAND_HPP
