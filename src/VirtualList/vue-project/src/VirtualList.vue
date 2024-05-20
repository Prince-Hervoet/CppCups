<template>
  <div ref="mainContainerRef" class="main-container" @scroll="scrollHandler">
    <div class="height-container" :style="{ height: `${totalHeight}px` }"></div>
    <div
      class="list-container"
      :style="{
        transform: `translateY(${scrollOffset}px)`,
        '--item-height-var': `${props.itemHeight}px`,
      }"
    >
      <div v-for="data in localData" class="item-container">
        <slot :data="data"></slot>
      </div>
    </div>
  </div>
</template>

<script setup>
import { computed, ref } from "vue";

const props = defineProps(["itemHeight", "viewHeight", "data", "bufferCount"]);
// 滚动的距离
const scrollTop = ref(0);
// 主div DOM的引用
const mainContainerRef = ref(null);

// 列表总高度
const totalHeight = computed(() => {
  if (!props.data) return 0;
  return props.data.length * props.itemHeight;
});

// 需要展示的数量
const viewCount = computed(() => {
  return Math.ceil(props.viewHeight / props.itemHeight);
});

// 起始index
const startIndex = computed(() => {
  return Math.floor(scrollTop.value / props.itemHeight);
});

// 终止index
const endIndex = computed(() => {
  return startIndex.value + viewCount.value + (props.bufferCount ?? 0);
});

// 当前列表数据
const localData = computed(() => {
  if (!props.data) return [];
  const nEndIndex =
    endIndex.value >= props.data.length
      ? props.data.length - 1
      : endIndex.value;
  return props.data.slice(startIndex.value, nEndIndex);
});

// 滚动修补偏移量
const scrollOffset = computed(() => {
  const criticalValue = (props.bufferCount ?? 0) * props.itemHeight;
  return scrollTop.value <= criticalValue
    ? 0
    : scrollTop.value - (scrollTop.value % props.itemHeight) - criticalValue;
});

const scrollHandler = () => {
  if (!mainContainerRef.value) return;
  const nScrollTop = mainContainerRef.value.scrollTop ?? 0;
  scrollTop.value = nScrollTop;
};
</script>

<style scoped>
.main-container {
  width: 100%;
  height: 100%;
  position: relative;
  overflow: auto;
}

.height-container {
  top: 0;
  left: 0;
  position: absolute;
  width: 100%;
}

.list-container {
  top: 0;
  left: 0;
  position: absolute;
  width: 100%;
}

.item-container {
  width: 100%;
  height: var(--item-height-var);
  box-sizing: border-box;
}
</style>
