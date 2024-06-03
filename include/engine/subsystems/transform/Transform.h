/**************************************************************************************************
 * @file   Transform.h
 * @author Valentin Dumitru
 * @date   16/02/2024
 * @brief  Add description of this file if needed @TODO
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/
#pragma once
#include <cmath>

#include "TransformTypes.h"
#include "engine/core/math/geometry/GeometryTypes.h"
#include "engine/subsystems/ingame-debug/EntityStatsManager.h"
#include "engine/subsystems/renderer/mesh/MeshTypes.h"

namespace GLESC::Transform {
    enum class Axis {
        X = 0,
        Y = 1,
        Z = 2
    };
    enum class RotationAxis {
        Pitch = static_cast<int>(Axis::X),
        Yaw = static_cast<int>(Axis::Y),
        Roll = static_cast<int>(Axis::Z)
    };
    struct Transform{
        static Math::Direction worldUp;
        static Math::Direction worldRight;
        static Math::Direction worldForward;

        Transform(){
            modelMat.makeModelMatrix(position, rotationDegrees.toRads(), scale);
        };


        Transform(Position position, Rotation rotation, Scale scale);

        const Position& getPosition() const {
            return position;
        }

        const Rotation& getRotation() const {
            return rotationDegrees;
        }

        const Scale& getScale() const {
            return scale;
        }

        void setPosition(const Position& position) {
            dirty = true;
            this->position = position;
        }

        void setRotation(const Rotation& rotation) {
            dirty = true;
            this->rotationDegrees = rotation;
        }

        void setScale(const Scale& scale) {
            dirty = true;
            this->scale = scale;
        }

        void setPosition(Axis axis, PosComp value) {
            dirty = true;
            int index = static_cast<int>(axis);
            position.set(index, value);
        }

        void setRotation(RotationAxis axis, RotComp value) {
            dirty = true;
            int index = static_cast<int>(axis);
            rotationDegrees.set(index, value);
        }

        void setScale(Axis axis, ScaleComp value) {
            dirty = true;
            int index = static_cast<int>(axis);
            scale.set(index, value);
        }


        void addPosition(const Position& position) {
            setPosition(this->position + position);
        }

        void addRotation(const Rotation& rotation) {
            setRotation(this->rotationDegrees + rotation);
        }

        void addScale(const Scale& scale) {
            setScale(this->scale + scale);
        }

        void addPosition(Axis axis, PosComp value) {
            setPosition(axis, position.get(static_cast<int>(axis)) + value);
        }

        void addRotation(RotationAxis axis, RotComp value) {
            setRotation(axis, rotationDegrees.get(static_cast<int>(axis)) + value);
        }

        void addScale(Axis axis, ScaleComp value) {
            setScale(axis, scale.get(static_cast<int>(axis)) + value);
        }

        Render::Model getModelMatrix() const {
            if (dirty) {
                modelMat.makeModelMatrix(position, rotationDegrees.toRads(), scale);
                dirty = false;
            }
            return modelMat;
        }

        Render::TranslateMat getTranslationMatrix() const {
            if (dirty) {
                translateMat.makeModelMatrix(position, Vec3F{0,0,0}, Vec3F{0,0,0});
                dirty = false;
            }
            return translateMat;
        }

        Render::RotateMat getRotationMatrix() const {
            if (dirty) {
                rotateMat.makeModelMatrix(Vec3F{0,0,0}, rotationDegrees.toRads(), Vec3F{0,0,0});
                dirty = false;
            }
            return rotateMat;
        }

        Render::ScaleMat getScaleMatrix() const {
            if (dirty) {
                scaleMat.makeModelMatrix(Vec3F{0,0,0}, Vec3F{0,0,0}, scale);
                dirty = false;
            }
            return scaleMat;
        }

        [[nodiscard]] std::vector<EntityStatsManager::Value> getDebuggingValues() {
            std::vector<EntityStatsManager::Value> values;

            EntityStatsManager::Value positionValue;
            positionValue.name = "Position";
            positionValue.data = reinterpret_cast<void*>(&position);
            positionValue.valueDirty = &dirty;
            positionValue.type = EntityStatsManager::ValueType::VEC3F;
            positionValue.isModifiable = true;
            positionValue.usesSlider = false;
            values.push_back(positionValue);

            EntityStatsManager::Value rotationValue;
            rotationValue.name = "Rotation";
            rotationValue.data = reinterpret_cast<void*>(&rotationDegrees);
            positionValue.valueDirty = &dirty;
            rotationValue.type = EntityStatsManager::ValueType::VEC3F;
            rotationValue.isModifiable = true;
            rotationValue.usesSlider = true;
            rotationValue.min = -360.0f;
            rotationValue.max = 360.0f;
            values.push_back(rotationValue);

            EntityStatsManager::Value scaleValue;
            scaleValue.name = "Scale";
            scaleValue.data = reinterpret_cast<void*>(&scale);
            positionValue.valueDirty = &dirty;
            scaleValue.type = EntityStatsManager::ValueType::VEC3F;
            scaleValue.isModifiable = true;
            scaleValue.usesSlider = true;
            scaleValue.min = -10.0f;
            scaleValue.max = 10.0f;
            values.push_back(scaleValue);

            return values;
        }

        [[nodiscard]] Math::Direction forward() const;

        [[nodiscard]] Math::Direction right() const;

        [[nodiscard]] Math::Direction up() const;

        [[nodiscard]] bool operator==(const Transform& other) const;

        [[nodiscard]] std::string toString() const;

    private:
        Math::Direction calculateForward() const;
        Math::Direction calculateRight() const;
        Math::Direction calculateUp() const;

        Position position = Position(0.0f, 0.0f, 0.0f);
        Rotation rotationDegrees = Rotation(0.0f, 0.0f, 0.0f);
        Scale scale = Scale(1.0f, 1.0f, 1.0f);

        Math::Direction forwardDirection;
        Math::Direction rightDirection;
        Math::Direction upDirection;

        // Mutable because we want to return it lazily
        mutable Render::Model modelMat;
        mutable Render::TranslateMat translateMat;
        mutable Render::RotateMat rotateMat;
        mutable Render::ScaleMat scaleMat;
        mutable bool dirty = true;
    };

    class Transformer {
    public:
        static void translateMesh(Render::ColorMesh& mesh, const Position& translation);
        static void transformMesh(Render::ColorMesh& mesh, const Transform& transform);
        static Math::BoundingVolume transformBoundingVolume(const Math::BoundingVolume& boundingVolume,
        const Transform& transform);
        static Math::BoundingVolume transformBoundingVolume(const Math::BoundingVolume& boundingVolume,
                                                            const Render::Model& matrix);

        static Position transformPosition(const Position& position, const Render::Model& matrix);
        static Position clipToNDC(const HomogeneousPosition& clipPos);
        static Position NDCToViewport(const Position& clipPos, float vpWidth, float vpHeight);
        static Position worldToViewport(const Position& worldPos, const Render::VP& viewProj, float vpWidth,
                                        float vpHeight);

    private:
        static void transformMesh(Render::ColorMesh& mesh, const Mat4F& matrix);
    }; // class Transformer

    struct Interpolator {
        Interpolator() = default;
        void pushTransform(const Transform& transform);
        Transform interpolate(float alphaParam) const;

    private:
        Transform currentTransform = Transform();
        Transform lastTransform = Transform();
    }; // struct Interpolator
} // namespace GLESC
