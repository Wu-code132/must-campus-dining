import org.springframework.web.bind.annotation.*;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

@RestController
@RequestMapping("/api/feedback")
public class FeedbackController {

    private static final List<Feedback> mockDatabase = new ArrayList<>();

    @PostMapping("/create")
    public String createFeedback(@RequestBody Feedback submitForm, 
                                 @RequestHeader(value = "Authorization", defaultValue = "session_user_007") String contextUserId) {
        
        if (submitForm.getContent() == null || submitForm.getContent().trim().isEmpty()) {
            return "Error: Feedback content cannot be empty!"; 
        }
        
        if (submitForm.getContent().length() > 500) {
            return "Error: Feedback content cannot exceed 500 characters!"; 
        }

        Feedback newFeedback = new Feedback();
        newFeedback.setFeedbackId(UUID.randomUUID().toString());
        newFeedback.setDishId(submitForm.getDishId());
        newFeedback.setContent(submitForm.getContent());
        newFeedback.setUserId(contextUserId); 
        newFeedback.setStatus("Submitted"); 
        newFeedback.setCreateTime(LocalDateTime.now());

        mockDatabase.add(newFeedback);
        
        return "Success: Feedback submitted successfully!";
    }

    @GetMapping("/myList")
    public List<Feedback> getMyList(@RequestHeader(value = "Authorization", defaultValue = "session_user_007") String contextUserId) {
        List<Feedback> myFeedbacks = new ArrayList<>();
        for (Feedback fb : mockDatabase) {
            if (fb.getUserId().equals(contextUserId)) {
                myFeedbacks.add(fb);
            }
        }
        return myFeedbacks;
    }

    @GetMapping("/detail/{feedbackId}")
    public Object getDetail(@PathVariable String feedbackId, 
                            @RequestHeader(value = "Authorization", defaultValue = "session_user_007") String contextUserId) {
        
        for (Feedback fb : mockDatabase) {
            if (fb.getFeedbackId().equals(feedbackId)) {
                if (!fb.getUserId().equals(contextUserId)) {
                    return "Error: Access denied! You cannot view others' feedback.";
                }
                return fb; 
            }
        }
        return "Error: Record not found";
    }
}
