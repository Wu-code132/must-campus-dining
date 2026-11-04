import java.time.LocalDateTime;

public class Feedback {
    private String feedbackId;
    private String dishId;
    private String userId;
    private String content;
    private String status;
    private LocalDateTime createTime;

    public String getFeedbackId() { return feedbackId; }
    public void setFeedbackId(String feedbackId) { this.feedbackId = feedbackId; }

    public String getDishId() { return dishId; }
    public void setDishId(String dishId) { this.dishId = dishId; }

    public String getUserId() { return userId; }
    public void setUserId(String userId) { this.userId = userId; }

    public String getContent() { return content; }
    public void setContent(String content) { this.content = content; }

    public String getStatus() { return status; }
    public void setStatus(String status) { this.status = status; }

    public LocalDateTime getCreateTime() { return createTime; }
    public void setCreateTime(LocalDateTime createTime) { this.createTime = createTime; }
}
