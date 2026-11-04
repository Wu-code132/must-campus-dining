CREATE TABLE Feedback (
    feedback_id VARCHAR(50) PRIMARY KEY,
    dish_id VARCHAR(20) NOT NULL,        
    user_id VARCHAR(50) NOT NULL,        
    content TEXT NOT NULL,               
    status VARCHAR(20) DEFAULT 'Submitted', 
    create_time DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE FollowUpRecord (
    record_id VARCHAR(50) PRIMARY KEY,
    feedback_id VARCHAR(50) NOT NULL,
    admin_uid VARCHAR(50) NOT NULL,      
    action_note TEXT,                    
    create_time DATETIME DEFAULT CURRENT_TIMESTAMP
);
