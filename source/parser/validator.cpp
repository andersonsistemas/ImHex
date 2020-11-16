#include "parser/validator.hpp"

#include <unordered_set>
#include <string>

namespace hex::lang {

    Validator::Validator() {

    }

    bool Validator::validate(const std::vector<ASTNode*>& ast) {

        std::unordered_set<std::string> typeNames;

        for (const auto &node : ast) {
            switch (node->getType()) {
                case ASTNode::Type::VariableDecl:
                {
                    // Check for duplicate variable names
                    auto varDeclNode = static_cast<ASTNodeVariableDecl*>(node);
                    if (!typeNames.insert(varDeclNode->getVariableName()).second)
                        return false;
                }
                    break;
                case ASTNode::Type::TypeDecl:
                {
                    // Check for duplicate type names
                    auto typeDeclNode = static_cast<ASTNodeTypeDecl*>(node);
                    if (!typeNames.insert(typeDeclNode->getTypeName()).second)
                        return false;
                }
                break;
                case ASTNode::Type::Struct:
                {
                    // Check for duplicate type name
                    auto structNode = static_cast<ASTNodeStruct*>(node);
                    if (!typeNames.insert(structNode->getName()).second)
                        return false;

                    // Check for duplicate member names
                    std::unordered_set<std::string> memberNames;
                    for (const auto &member : structNode->getNodes())
                        if (!memberNames.insert(static_cast<ASTNodeVariableDecl*>(member)->getVariableName()).second)
                            return false;
                }
                break;
                case ASTNode::Type::Enum:
                {
                    // Check for duplicate type name
                    auto enumNode = static_cast<ASTNodeEnum*>(node);
                    if (!typeNames.insert(enumNode->getName()).second)
                        return false;

                    // Check for duplicate constant names
                    std::unordered_set<std::string> constantNames;
                    for (const auto &[value, name] : enumNode->getValues())
                        if (!constantNames.insert(name).second)
                            return false;
                }
                break;
            }
        }

        return true;
    }

}