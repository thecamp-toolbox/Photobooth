  </div>

  <hr>

  <footer class="container-fluid text-center">
    <a href="<?= url() ?>">&copy; <?= date('Y') ?> / <?= $site->title() ?></a>

    <?php if ($about = page('about')): ?>
    <?php endif ?>
  </footer>

</body>
</html>
