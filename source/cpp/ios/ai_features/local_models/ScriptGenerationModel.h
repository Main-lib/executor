#pragma once

#include "LocalModelBase.h"
#include <string>
#include <vector>
#include <functional>

namespace iOS {
namespace AIFeatures {
namespace LocalModels {

/**
 * @enum ScriptCategory
 * @brief Categories of scripts that can be generated
 */
enum class ScriptCategory {
    GENERAL,
    GUI,
    GAMEPLAY,
    UTILITY,
    NETWORKING,
    OPTIMIZATION,
    CUSTOM
};

/**
 * @class ScriptGenerationModel
 * @brief Model for generating and enhancing scripts
 * 
 * This model generates Lua scripts based on natural language prompts
 * and can enhance existing scripts with additional functionality.
 */
class ScriptGenerationModel : public LocalModelBase {
public:
    /**
     * @struct GeneratedScript
     * @brief Represents a generated script with metadata
     */
    struct GeneratedScript {
        std::string m_code;           // The actual script code
        ScriptCategory m_category;    // Category of the script
        float m_confidence;           // Confidence score (0.0-1.0)
        std::string m_description;    // Description of what the script does
        
        GeneratedScript() 
            : m_category(ScriptCategory::GENERAL), m_confidence(0.0f) {}
        
        GeneratedScript(const std::string& code, ScriptCategory category = ScriptCategory::GENERAL,
                       float confidence = 1.0f, const std::string& description = "")
            : m_code(code), m_category(category), 
              m_confidence(confidence), m_description(description) {}
    };

    /**
     * @brief Constructor
     */
    ScriptGenerationModel();
    
    /**
     * @brief Destructor
     */
    virtual ~ScriptGenerationModel();
    
    /**
     * @brief Initialize the model
     * @param path Path to model files
     * @return True if initialization succeeded
     */
    bool Initialize(const std::string& path);
    
    /**
     * @brief Load the model from disk
     * @return True if load succeeded
     */
    bool Load();
    
    /**
     * @brief Save the model to disk
     * @return True if save succeeded
     */
    bool Save();
    
    /**
     * @brief Train the model
     * @return True if training succeeded
     */
    bool Train();
    
    /**
     * @brief Generate a script from a prompt
     * @param prompt User prompt
     * @return Generated script
     */
    std::string GenerateScript(const std::string& prompt);
    
    /**
     * @brief Generate a script from a prompt with specific category
     * @param prompt User prompt
     * @param category Script category
     * @return Generated script
     */
    std::string GenerateScript(const std::string& prompt, ScriptCategory category);

    /**
     * @brief Generate a script with metadata
     * @param prompt User prompt
     * @param category Script category
     * @return GeneratedScript object with code and metadata
     */
    GeneratedScript GenerateScriptWithMetadata(const std::string& prompt, 
                                             ScriptCategory category = ScriptCategory::GENERAL);
    
    /**
     * @brief Enhance an existing script
     * @param script Original script
     * @param prompt Enhancement instructions
     * @return Enhanced script
     */
    std::string EnhanceScript(const std::string& script, const std::string& prompt);
    
    /**
     * @brief Generate script asynchronously
     * @param prompt User prompt
     * @param callback Callback to invoke when generation completes
     */
    void GenerateScriptAsync(const std::string& prompt, std::function<void(const std::string&)> callback);
    
    /**
     * @brief Enhance script asynchronously
     * @param script Original script
     * @param prompt Enhancement instructions
     * @param callback Callback to invoke when enhancement completes
     */
    void EnhanceScriptAsync(const std::string& script, const std::string& prompt, 
                           std::function<void(const std::string&)> callback);
    
    /**
     * @brief Convert script category to string
     * @param category Script category
     * @return String representation
     */
    static std::string CategoryToString(ScriptCategory category);
    
    /**
     * @brief Convert string to script category
     * @param str String representation
     * @return Script category
     */
    static ScriptCategory StringToCategory(const std::string& str);

protected:
    // Implement the pure virtual methods from LocalModelBase
    virtual bool InitializeModel() override;
    virtual bool TrainModel(TrainingProgressCallback progressCallback = nullptr) override;
    virtual std::string PredictInternal(const std::string& input) override;
    virtual std::vector<float> FeaturizeInput(const std::string& input) override;
    virtual std::string ProcessOutput(const std::vector<float>& output) override;
};

} // namespace LocalModels
} // namespace AIFeatures
} // namespace iOS
