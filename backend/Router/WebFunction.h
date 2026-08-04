#pragma once

#include <glaze/glaze.hpp>

#include "../AppContext/AppContext.h"

template <typename Ty>
concept JsonSupported = std::is_same_v<Ty, void> ||
    (glz::write_supported<Ty, glz::JSON> && glz::read_supported<Ty, glz::JSON>);

template <JsonSupported InputTy, JsonSupported OutputTy>
struct WebFunction {
    using InputType = InputTy;
    using OutputType = OutputTy;

    virtual OutputTy Invoke(AppContext& ctx, const InputTy& object) = 0;
    virtual ~WebFunction() = default;
};

template <JsonSupported OutputTy>
struct WebFunction<void, OutputTy> {
    using InputType = void;
    using OutputType = OutputTy;

    virtual OutputTy Invoke(AppContext& ctx) = 0;
    virtual ~WebFunction() = default;
};

class WebFunctionContainer {
public:
    virtual  std::optional<std::string> InvokeInner(AppContext& ctx, const char* json) = 0;
    virtual ~WebFunctionContainer() = default;
};

template <typename FunctionTy, typename InputTy = FunctionTy::InputType, typename OutputTy = FunctionTy::OutputType>
    requires std::derived_from<FunctionTy, WebFunction<InputTy, OutputTy>>
class WebFunctionContainerImpl final : public WebFunctionContainer {
    FunctionTy m_InnerFunction{};

public:
    std::optional<std::string> InvokeInner(AppContext& ctx, const char* json) override {
        // Does not require any JSON conversion if it takes in void
        if constexpr (std::is_void_v<InputTy>) {
            // Sends a warning if an object was provided
            if (json != nullptr) {
                ctx.LogError("Input parameter was provided to a void web function");
                return std::nullopt;
            }

            // Serializes the outputted object (if there is one, and returns it)
            if constexpr (std::is_void_v<OutputTy>) {
                m_InnerFunction.Invoke(ctx);
                return std::nullopt;
            } else {
                OutputTy output = m_InnerFunction.Invoke(ctx);

                std::string buffer{};
                if (const glz::error_ctx ec = glz::write_json(output, buffer)) {
                    ctx.LogError("Failed to write json due to [", glz::format_error(ec), ']');
                    return std::nullopt;
                }

                return buffer;
            }

        } else {
            // Checks there is an input object provided
            if (json == nullptr) {
                ctx.LogError("No input parameter was provided to a web function.");
                return std::nullopt;
            }

            // Constructs the object from the JSON
            InputTy object{};
            if (const glz::error_ctx ec = glz::read_json(object, json)) {
                ctx.LogError("Failed to parse json due to [", glz::format_error(ec), ']');
                return std::nullopt;
            }

            // Serializes the outputted object (if there is one, and returns it)
            if constexpr (std::is_void_v<OutputTy>) {
                m_InnerFunction.Invoke(ctx, object);
                return std::nullopt;
            } else {
                OutputTy output = m_InnerFunction.Invoke(ctx, object);

                std::string buffer{};
                if (const glz::error_ctx ec = glz::write_json(output, buffer)) {
                    ctx.LogError("Failed to write json due to [", glz::format_error(ec), ']');
                    return std::nullopt;
                }

                return buffer;
            }
        }
    }
};
