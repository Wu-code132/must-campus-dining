<template>
  <div style="padding: 20px; max-width: 600px; margin: auto;">
    <h3>Submit Feedback</h3>
    <div style="margin-bottom: 15px;">
      <input v-model="dishId" placeholder="Enter Dish ID (e.g., D001)" style="width: 100%; padding: 8px; margin-bottom: 10px;" />
      <textarea v-model="content" rows="4" placeholder="Write your feedback here..." style="width: 100%; padding: 8px;"></textarea>
      <button @click="submitFeedback" :disabled="loading" style="margin-top: 10px; padding: 10px 20px;">
        {{ loading ? 'Submitting...' : 'Submit' }}
      </button>
      <p v-if="message" style="color: red;">{{ message }}</p >
    </div>

    <h3>My Feedback List</h3>
    
    <div v-if="listLoading">Loading...</div>
    
    <div v-else-if="listError">
      <p>Failed to load.</p >
      <button @click="fetchMyList">Retry</button>
    </div>
    
    <div v-else-if="myList.length === 0">
      <p>No feedback records found.</p >
    </div>
    
    <div v-else>
      <div v-for="item in myList" :key="item.feedbackId" style="border: 1px solid #ccc; padding: 10px; margin-bottom: 10px;">
        <p><strong>Dish ID:</strong> {{ item.dishId }}</p >
        <p><strong>Content:</strong> {{ item.content }}</p >
        <p><strong>Status:</strong> {{ item.status }}</p >
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import axios from 'axios'

const dishId = ref('')
const content = ref('')
const loading = ref(false)
const message = ref('')

const myList = ref([])
const listLoading = ref(true)
const listError = ref(false)

const fetchMyList = async () => {
  listLoading.value = true
  listError.value = false
  try {
    const res = await axios.get('/api/feedback/myList')
    myList.value = res.data
  } catch (error) {
    listError.value = true
  } finally {
    listLoading.value = false
  }
}

const submitFeedback = async () => {
  if (!content.value.trim()) {
    message.value = 'Content cannot be empty!'
    return
  }
  if (content.value.length > 500) {
    message.value = 'Cannot exceed 500 characters!'
    return
  }

  loading.value = true
  message.value = ''
  try {
    await axios.post('/api/feedback/create', {
      dishId: dishId.value,
      content: content.value
    })
    alert('Success!')
    content.value = ''
    fetchMyList()
  } catch (error) {
    message.value = 'Submit failed.'
  } finally {
    loading.value = false
  }
}

onMounted(() => {
  fetchMyList()
})
</script>
