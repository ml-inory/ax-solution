/**************************************************************************************************
 *
 * Copyright (c) 2019-2023 Axera Semiconductor (Shanghai) Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor (Shanghai) Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor (Shanghai) Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include "err.hpp"
#include "node.hpp"

#include <thread>

namespace ax
{
    typedef std::shared_ptr<Node>       NodePtr;

    /// @brief Pipeline base class
    class AX_Pipeline
    {
    public:
        AX_Pipeline():
            m_hasInit(false),
            m_hasStart(false)
        { }

        virtual ~AX_Pipeline() {}

        virtual int Init() = 0;

        /// @brief start nodes
        virtual int Start()
        {
            if (!m_hasInit)
                return AX_ERR_INIT_FAIL;
            if (m_hasStart)
                return AX_SUCCESS;

            // Run
            for (auto& node : m_nodes)
            {
                node->SetRunning();

                std::thread t(&Node::Run, node);
                t.detach();
            }

            m_hasStart = true;
            return AX_SUCCESS;
        }

        /// @brief stop nodes
        virtual int Stop()
        {
            if (!m_hasInit)
                return AX_ERR_NOT_INIT;
            if (!m_hasStart)
                return AX_SUCCESS;

            for (const auto& node : m_nodes)
            {
                node->Stop();
            }
            m_hasStart = false;
            return AX_SUCCESS;
        }

        bool AddNode(NodePtr new_node)
        {
            if (FindNode(new_node->name()) != nullptr)
                return false;

            if (0 != new_node->Init())
            {
                return false;
            }

            m_nodes.push_back(new_node);
            return true;
        }

        NodePtr GetNode(int index)
        {
            if (index >= m_nodes.size())
                return nullptr;

            return m_nodes[index];
        }

        NodePtr FindNode(const std::string node_name)
        {
            for (const auto& node : m_nodes)
            {
                if (node->name() == node_name)
                    return node;
            }
            return nullptr;
        }

        int GetNodeNum() const {
            return m_nodes.size();
        }

        std::shared_ptr<InputPort> GetInputPort() const 
        {
            if (!m_hasInit)
            {
                return nullptr;
            }
                
            for (const auto& node : m_nodes)
            {
                for (size_t i = 0; i < node->GetInputPortNum(); i++)
                {
                    auto iport = node->GetInputPort(i);
                    if (!iport->has_stream())
                    {
                        return iport;
                    }
                }
            }
            return nullptr;
        }

        std::shared_ptr<OutputPort> GetOutputPort() const 
        {
            if (!m_hasInit)
            {
                return nullptr;
            }
                
            for (const auto& node : m_nodes)
            {
                for (size_t i = 0; i < node->GetOutputPortNum(); i++)
                {
                    auto oport = node->GetOutputPort(i);
                    if (!oport->has_stream())
                        return oport;
                }
            }
            return nullptr;
        }

    protected:
        std::vector<NodePtr> m_nodes;
        bool m_hasInit;
        bool m_hasStart;
    };
}
