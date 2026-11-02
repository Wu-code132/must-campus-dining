void submitFeedback(string uid, string content) {
string status = "Submitted";
cout << status << endl;
}

void queryFeedback(string uid, int fid, string submitter_uid) {
    if (uid != submitter_uid) {
        cout << "Access denied" << endl;
        return;
    }
    cout << "Show feedback details" << endl;
}

void acceptFeedback(int fid, string current_status) {
    if (current_status == "Submitted") {
        current_status = "Processing";
        cout << current_status << endl;
    }
}

void replyFeedback(int fid, string admin_uid, string note) {
    cout << "Follow-up saved" << endl;
}

void resolveFeedback(int fid, string current_status) {
    if (current_status == "Processing") {
        current_status = "Resolved";
        cout << current_status << endl;
    }
}




struct Feedback {
    string user_id;          
    string content;          
    string status;           
    string creation_time;    
};

struct FollowUpRecord {
    string feedback_id;      
    string operator_id;      
    string record_time;      
};

void createData() {
    Feedback fb;
    fb.user_id = "test-user01";          
    fb.content = "The food is too salty";
    fb.status = "Submitted";             
    fb.creation_time = "2026-11-02 10:00"; 

    FollowUpRecord record;
    record.feedback_id = "F1001";        
    record.operator_id = "test-admin01"; 
    record.record_time = "2026-11-02 11:00"; 
}


struct Feedback {
    string status;
    int followUpCount;
};

void initFeedback(Feedback &fb) {
    fb.status = "Submitted";
    fb.followUpCount = 0;
}

void acceptFeedback(Feedback &fb) {
    if (fb.status == "Submitted") {
        fb.status = "Processing";
    }
}

void resolveFeedback(Feedback &fb) {
    if (fb.status == "Processing") {
        fb.status = "Resolved";
    }
}



void testApi() {
    string token = "Bearer test-user01";
    string txt = "The food is too salty.";
    
    if (token == "Bearer test-user01") {
        cout << "User submitted: " << txt << endl;
        cout << "status: Submitted" << endl;
        cout << "follow_ups: 0" << endl;
    }

    string adminToken = "Bearer test-admin01";
    int id = 1001;
    
    if (adminToken == "Bearer test-admin01") {
        cout << "Admin accepted id: " << id << endl;
        cout << "status: Processing" << endl;
    }
}
